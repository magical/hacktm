/*
 * This program is useful for testing and watching the algorithm learn
 * sequences of inputs. You can feed to the system four different
 * bitmaps (test1.xbm, test2.xbm, test3.xbm, test4.xbm) using the keys
 * 'z', 'x', 'c' or 'v'.
 *
 * Visualization is done trought gnuplot, so the suggested usage is:
 *  ./testNode | gnuplot
 *
 * Input Bitmap files are in xbm format, easily editable by the X11
 * "bitmap" core utility.
 */
#include "BitVector.h"
#include "HackTM.h"
#include "SpatialPooler.h"
#include "TemporalPooler.h"
#include "Introspection.h"
#include "xbmlib.h"

#include "test1.xbm"
#include "test2.xbm"
#include "test3.xbm"
#include "test4.xbm"

using namespace hacktm;

#define CELLSPERCOL 4


main()
{
  struct xbmlib::xbmImage bmp1, bmp2, bmp3, bmp4;
  BitVector bv1, bv2, bv3, bv4;

  LOAD_XBM(&bmp1, test1);
  xbmlib::setBitVectorFromXbm(bmp1, bv1);
  LOAD_XBM(&bmp2, test2);
  xbmlib::setBitVectorFromXbm(bmp2, bv2);
  LOAD_XBM(&bmp3, test3);
  xbmlib::setBitVectorFromXbm(bmp3, bv3);
  LOAD_XBM(&bmp4, test4);
  xbmlib::setBitVectorFromXbm(bmp4, bv4);


  htmtime_t cur = 0;
  htmtime_t prev = 1;

  std::list<hacktm::id_t> actColumns;
  Vector input(3, 100);
  Vector columns(3,10);
  Space inputSpace(input);
  Space columnSpace(columns);
  SpatialPooler sp(&inputSpace, &columnSpace);
  TemporalPooler tp(&columnSpace, CELLSPERCOL);
  BitVector output(columnSpace.getSize() * CELLSPERCOL);

  /* Disable debug flags. */
  hacktmdebug::Flags = 0;

  while ( 1 ) {
    cur = ( cur + 1 ) % 2;
    prev = (prev + 1) % 2;
    char c;
    std::cin >> c;
    switch ( c ) {
    case 'z' : 
      sp.run(bv1, actColumns);
      break;
    case 'x':
      sp.run(bv2, actColumns);
      break;
    case 'c':
      sp.run(bv3, actColumns);
      break;
    case 'v':
      sp.run(bv4, actColumns);
      break;
    default:
      continue;
    }
    tp.run(cur, prev, actColumns, output);

    Introspection is;
    const Space *cellSpace = is.getTemporalPoolerCISpace(&tp)->getSpace();
    const BitVector *activeState = is.getCellsStateActiveState(is.getTemporalPoolerCellsState(&tp), cur);
    const BitVector *predictiveState = is.getCellsStatePredictiveState(is.getTemporalPoolerCellsState(&tp), cur);
    const BitVector *learnState = is.getCellsStateLearnState(is.getTemporalPoolerCellsState(&tp));
    Vector v(cellSpace->getDimension());

    std::cout << "clear" << std::endl;
    std::cout << "set grid on " << std::endl;
    std::cout << "set key off" << std::endl;
    std::cout << "set multiplot layout 4,4" << std::endl;

    for ( id_t id = 0; id < cellSpace->getSize(); id++ ) {

      if ( (id % columnSpace.getSize()) == 0 ) {
	if ( id != 0 )
	  std::cout << "e" << std::endl;
	else
	  std::cout << "set title 'Active State'" << std::endl;
	std::cout << "splot [0:10][0:10][0:10] '-' using 1:2:3 " << std::endl;
      }
      if ( activeState->test(id) ) {
	cellSpace->setVectorFromId(id, v);
	for ( unsigned i = 0; i < cellSpace->getDimension(); i++ )
	  std::cout << v[i] << " ";
	std::cout << std::endl;
      }
    }
    std::cout << "e" << std::endl;


    for ( id_t id = 0; id < cellSpace->getSize(); id++ ) {

      if ( (id % columnSpace.getSize()) == 0 ) {
	if ( id != 0 )
	  std::cout << "e" << std::endl;
	else
	  std::cout << "set title 'Predictive State'" << std::endl;
	std::cout << "splot [0:10][0:10][0:10] '-' using 1:2:3 " << std::endl;
      }
      if ( predictiveState->test(id) ) {
	cellSpace->setVectorFromId(id, v);
	for ( unsigned i = 0; i < cellSpace->getDimension(); i++ )
	  std::cout << v[i] << " ";
	std::cout << std::endl;
      }
    }
    std::cout << "e" << std::endl;

    for ( id_t id = 0; id < cellSpace->getSize(); id++ ) {

      if ( (id % columnSpace.getSize()) == 0 ) {
	if ( id != 0 )
	  std::cout << "e" << std::endl;
	else
	  std::cout << "set title 'Learn State'" << std::endl;
	std::cout << "splot [0:10][0:10][0:10] '-' using 1:2:3 " << std::endl;
      }
      if ( learnState->test(id) ) {
	cellSpace->setVectorFromId(id, v);
	for ( unsigned i = 0; i < cellSpace->getDimension(); i++ )
	  std::cout << v[i] << " ";
	std::cout << std::endl;
      }
    }
    std::cout << "e" << std::endl;

    for ( id_t id = 0; id < cellSpace->getSize(); id++ ) {

      if ( (id % columnSpace.getSize()) == 0 ) {
	if ( id != 0 )
	  std::cout << "e" << std::endl;
	else
	  std::cout << "set title 'Output'" << std::endl;
	std::cout << "splot [0:10][0:10][0:10] '-' using 1:2:3 " << std::endl;
      }
      if ( output.test(id) ) {
	cellSpace->setVectorFromId(id, v);
	for ( unsigned i = 0; i < cellSpace->getDimension(); i++ )
	  std::cout << v[i] << " ";
	std::cout << std::endl;
      }
    }
    std::cout << "e" << std::endl;

    std::cout << "unset multiplot" << std::endl;
  }
}
