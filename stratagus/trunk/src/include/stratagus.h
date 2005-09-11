//       _________ __                 __
//      /   _____//  |_____________ _/  |______     ____  __ __  ______
//      \_____  \\   __\_  __ \__  \\   __\__  \   / ___\|  |  \/  ___/
//      /        \|  |  |  | \// __ \|  |  / __ \_/ /_/  >  |  /\___ |
//     /_______  /|__|  |__|  (____  /__| (____  /\___  /|____//____  >
//             \/                  \/          \//_____/            \/
//  ______________________                           ______________________
//                        T H E   W A R   B E G I N S
//         Stratagus - A free fantasy real time strategy game engine
//
/**@name stratagus.h - The main header file. */
//
//      (c) Copyright 1998-2005 by Lutz Sammer and Jimmy Salmon
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; only version 2 of the License.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//      02111-1307, USA.
//
//      $Id$

#ifndef __STRATAGUS_H__
#define __STRATAGUS_H__

//@{

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/*============================================================================
==  Config definitions
============================================================================*/

// Dynamic loading.
//#define DYNAMIC_LOAD

/*============================================================================
==  Compiler repairs
============================================================================*/

#ifdef __GNUC__  // {

#if __GNUC__==2 && __GNUC_MINOR__==96
#warning "GCC 2.96 is not supported and buggy, downgrade to 2.95 or upgrade to 3.2 or later"
#endif

#if __GNUC__==3 && __GNUC_MINOR__<2
#warning "GCC versions before 3.2 are not supported, upgrade to 3.2 or later"
#endif

#if defined(__MINGW32__) && defined(DEBUG)
// GDB + MINGW doesn't like free(0)
#include <stdlib.h>
#define free(x) do { void* __x; __x = (x); if (__x) { free(__x); } } while (0)
#endif

#endif  // } __GNUC__

#ifdef _MSC_VER

#define WIN32_LEAN_AND_MEAN
#define NOUSER

#define inline __inline
#define alloca _alloca

#pragma warning(disable:4244)               /// Conversion from double to uchar
#pragma warning(disable:4761)               /// Integral size mismatch
#pragma warning(disable:4786)               /// Truncated to 255 chars
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#include <string.h>
#define strdup _strdup
#define strncasecmp strnicmp

#endif  // } _MSC_VER

/*============================================================================
==  Declarations
============================================================================*/

// This is needed to have recursive forward references

#if !defined(__STRUCT_PLAYER__) && !defined(DOXYGEN)
#define __STRUCT_PLAYER__
typedef struct _player_ Player;
#endif

#if !defined(__STRUCT_VIEWPORT__) && !defined(DOXYGEN)
#define __STRUCT_VIEWPORT__
typedef struct _viewport_ Viewport;
#endif

#if !defined(__STRUCT_MISSILETYPE__) && !defined(DOXYGEN)
#define __STRUCT_MISSILETYPE__
typedef struct _missile_type_ MissileType;
#endif

/*============================================================================
==  Debug definitions
============================================================================*/

	/**
	**  This simulates vararg macros.
	**  Example:
	**    DebugPrint("Test %d %d\n" _C_ 1 _C_ 2);
	*/
#define _C_  ,    /// Debug , to simulate vararg macros

	/// Print function in debug macros
#define PrintFunction() \
	do { fprintf(stdout, "%s:%d: ", __FILE__, __LINE__); } while (0)


#ifdef DEBUG  // {

	/**
	**  Assert a condition. If cond is not true abort with file,line.
	*/
#define Assert(cond)  do { if (!(cond)) { \
	fprintf(stderr, "Assertion failed at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
	abort(); }} while (0)

	/**
	**  Print debug information with function name.
	*/
#define DebugPrint(args) \
	do { PrintFunction(); fprintf(stdout, args); } while (0)

#else  // }{ DEBUG

#define Assert(cond)        /* disabled */
#define DebugPrint(args)    /* disabled */

#endif

#ifdef REFS_DEBUG  // {

	/**
	**  Assert a condition for references
	*/
#define RefsAssert(cond)  do { if (!(cond)) { \
	fprintf(stderr, "Assertion failed at %s:%d\n", __FILE__, __LINE__); \
	abort(); } } while (0)

#else  // }{ REFS_DEBUG

#define RefsAssert(cond)      /* disabled */

#endif  // } !REFS_DEBUG

/*============================================================================
==  Definitions
============================================================================*/

/*----------------------------------------------------------------------------
--  General
----------------------------------------------------------------------------*/

#define VERSION  "2.2-CVS"                 /// Engine version shown

#ifndef StratagusMajorVerion
	/// Stratagus major version
#define StratagusMajorVersion  2
	/// Stratagus minor version (maximal 99)
#define StratagusMinorVersion  2
	/// Stratagus patch level (maximal 99)
#define StratagusPatchLevel    0
	/// Stratagus version (1,2,3) -> 10203
#define StratagusVersion \
	(StratagusMajorVersion * 10000 + StratagusMinorVersion * 100 \
		+ StratagusPatchLevel)

	/// Stratagus printf format string
#define StratagusFormatString   "%d.%d.%d"
	/// Stratagus printf format arguments
#define StratagusFormatArgs(v)  (v) / 10000, ((v) / 100) % 100, (v) % 100
#endif

	/// Text string: Name, Version, Copyright
extern char NameLine[];

#ifndef STRATAGUS_LIB_PATH
#define STRATAGUS_LIB_PATH  "data"      /// Where to find the data files
#endif

#ifndef STRATAGUS_HOME_PATH
#ifdef __APPLE__
#define STRATAGUS_HOME_PATH "Library/Stratagus"
#else
 #define _VERSION_STRING1(m,n,p) m "-" #n "." #p
 #define _VERSION_STRING(m,n,p) _VERSION_STRING1(m,n,p)
 #define STRATAGUS_HOME_PATH _VERSION_STRING(".stratagus",StratagusMajorVersion,StratagusMinorVersion)
#endif
#endif

#define MAGIC_FOR_NEW_UNITS  33         /// How many percent of max mana for new units

/*----------------------------------------------------------------------------
--  Some limits
----------------------------------------------------------------------------*/

#define PlayerMax    16                 /// How many players are supported
#define UnitTypeMax  257                /// How many unit types supported
#define UpgradeMax   256                /// How many upgrades supported
#define UnitMax      2048               /// How many units supported

/*----------------------------------------------------------------------------
--  Screen
----------------------------------------------------------------------------*/

	// FIXME: this values should go into a general ui structure

#define DEFAULT_VIDEO_WIDTH   640       /// Default video width
#define DEFAULT_VIDEO_HEIGHT  480       /// Default video height

	/// Scrolling area (<= 15 y)
#define SCROLL_UP     15
	/// Scrolling area (>= VideoHeight - 16 y)
#define SCROLL_DOWN   (VideoHeight - 16)
	/// Scrolling area (<= 15 y)
#define SCROLL_LEFT   15
	/// Scrolling area (>= VideoWidth - 16 x)
#define SCROLL_RIGHT  (VideoWidth - 16)

	/// Mouse scrolling magnify
#define MOUSE_SCROLL_SPEED  3

	/// Keyboard scrolling magnify
#define KEY_SCROLL_SPEED  3

	/// Frames per second to display (original 30-40)
#define FRAMES_PER_SECOND  30  // 1/30s
	/// Game cycles per second to simulate (original 30-40)
#define CYCLES_PER_SECOND  30  // 1/30s 0.33ms

#define DEFAULT_START_ORDERS 4  // The number of Orders allocated on unit creation

/*----------------------------------------------------------------------------
--  stratagus.c
----------------------------------------------------------------------------*/

enum {
	TitleFlagCenter = 1 << 0,  /// Center Text
};

typedef struct _title_screen_label_ {
	char* Text;
	int Font;
	int Xofs;
	int Yofs;
	int Flags;
} TitleScreenLabel;

typedef struct _title_screen_ {
	char* File;
	char* Music;
	int Timeout;
	int Iterations;
	TitleScreenLabel** Labels;
} TitleScreen;

extern TitleScreen** TitleScreens;          /// File for title screen
extern char* GameName;                      /// Name of the game
extern char* MenuMusic;                     /// File for menu music
extern char* ClickMissile;                  /// Missile to show when you click
extern char* DamageMissile;                 /// Missile to show damage caused
extern char* StratagusLibPath;              /// Location of stratagus data

extern int SpeedBuild;                      /// Speed factor for building
extern int SpeedTrain;                      /// Speed factor for training
extern int SpeedUpgrade;                    /// Speed factor for upgrading
extern int SpeedResearch;                   /// Speed factor for researching

extern char UseHPForXp;                     /// true if gain XP by dealing damage, false if by killing.

extern unsigned long GameCycle;             /// Game simulation cycle counter
extern unsigned long FastForwardCycle;      /// Game Replay Fast Forward Counter

extern void LoadGame(char* filename);       /// Load saved game back
extern void SaveGame(const char* filename); /// Save game for later load
extern int SaveGameLoading;                 /// Save game is in progress of loading

extern void LoadAll(void);                  /// Load all data back

extern void Exit(int err);                  /// Exit stratagus
extern void ExitFatal(int err);             /// Exit stratagus with fatal error

extern void UpdateDisplay(void);            /// Game display update
extern void InitModules(void);              /// Initinalize all modules
extern void LoadModules(void);              /// Load all modules
extern void CleanModules(void);             /// Cleanup all modules
extern void DrawMapArea(void);              /// Draw the map area
extern void GameMainLoop(void);             /// Game main loop

	/// Show load progress
extern void ShowLoadProgress(const char* fmt,...);

/*============================================================================
==  Misc
============================================================================*/

// @todo configurable. maybe we could move it into one big global
// @todo settings struct?
	/// How many resources the player gets back if canceling building
#define CancelBuildingCostsFactor  75
	/// How many resources the player gets back if canceling training
#define CancelTrainingCostsFactor  100
	/// How many resources the player gets back if canceling research
#define CancelResearchCostsFactor  100
	/// How many resources the player gets back if canceling upgrade
#define CancelUpgradeCostsFactor   100

	/// How near a depot could be built to a resource
#define RESOURCE_DISTANCE  3

extern char* CompileOptions;
//@}

#endif // !__STRATAGUS_H__
