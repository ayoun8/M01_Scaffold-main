
//{{BLOCK(pokemon)

//======================================================================
//
//	pokemon, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 335 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 10720 + 2048 = 13280
//
//	Time-stamp: 2025-04-19, 21:38:07
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_POKEMON_H
#define GRIT_POKEMON_H

#define pokemonTilesLen 10720
extern const unsigned short pokemonTiles[5360];

#define pokemonMapLen 2048
extern const unsigned short pokemonMap[1024];

#define pokemonPalLen 512
extern const unsigned short pokemonPal[256];

#endif // GRIT_POKEMON_H

//}}BLOCK(pokemon)
