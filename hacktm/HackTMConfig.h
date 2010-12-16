#ifndef __HACKTMCONFIG_H__
#define __HACKTMCONFIG_H__

namespace htmconfig {

  /* The minimum permanence value at which a synapse is considered "connected" */
  const float connectedPerm = 0.2;

  /* Amount pf permanence value synapse are incremented/decremented during learning. */
  const float permanenceInc = 0.05;
  const float permanenceDec = 0.05;

  /* Minimum number of inputs that must be active for the column to be
     considered in Inhibition phase. */
  const unsigned minOverlap = 3;


  /* A parameter controlling the number of columns that will be
     winners after the inhibition step. */
  const unsigned desiredLocalActivity = 10;

}


/*
 * Debug and printout controls.
 */

namespace hacktmdebug {

  enum debug {
    Debug_None = 0,
    Debug_PrintOverlappingColumns = 1,
    Debug_PrintWinningColumns = 2,
    Debug_PrintInhibitionRadius = 4,
    Debug_All = 0xff
  };

  extern int DebugFlags;

}

#endif __HACKTMCONFIG_H__
