
//{{BLOCK(clouds)

//======================================================================
//
//	clouds, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 166 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 5312 + 2048 = 7872
//
//	Time-stamp: 2025-04-19, 22:37:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CLOUDS_H
#define GRIT_CLOUDS_H

#define cloudsTilesLen 5312
extern const unsigned short cloudsTiles[2656];

#define cloudsMapLen 2048
extern const unsigned short cloudsMap[1024];

#define cloudsPalLen 512
extern const unsigned short cloudsPal[256];

#endif // GRIT_CLOUDS_H

//}}BLOCK(clouds)
