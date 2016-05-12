#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>

#include "pointcollection.h"
#include "datapoint.h"

//! Load point attraction from CSV file
PointCollection* createPoints( QFile& file )
{
  PointCollection* p = new PointCollection();

  //chop first line;
  ( void ) file.readLine();

  while ( !file.atEnd() )
  {
    QString line = file.readLine();
    QStringList splitLine = line.split( ',' );

    int id1 = splitLine.at( 0 ).toInt();
    int id2 = splitLine.at( 1 ).toInt();
    double attraction = splitLine.at( 2 ).toDouble();

    if ( !p->dataPoint( id1 ) )
    {
      DataPoint* dP = new DataPoint( id1 );
      p->addDataPoint( dP );
    }
    if ( !p->dataPoint( id2 ) )
    {
      DataPoint* dP = new DataPoint( id2 );
      p->addDataPoint( dP );
    }
    p->setAttraction( id1, id2, attraction );
  }

  p->placeRandom();
  p->assignColors();

  return p;
}

PointCollection* createRandomPoints( int numPoints )
{
  PointCollection* p = new PointCollection();

  for ( int i = 0; i < numPoints; ++i )
  {
    DataPoint* dP = new DataPoint( i );
    p->addDataPoint( dP );
  }

  for ( int i = 0; i < numPoints; ++i )
  {
    for ( int j = 0; j < numPoints; ++j )
    {
      if ( i == j )
        continue;

      double attraction = ( ( double ) qrand() / ( RAND_MAX ) ) * 4 - 2;
      p->setAttraction( i, j, attraction );
    }
  }

  p->placeRandom();
  p->assignColors();

  return p;
}

int main( int argc, char* argv[] )
{
#if 0
  QFile file( "d:/data.csv" );
  if ( !file.open( QIODevice::ReadOnly ) )
  {
    qDebug() << file.errorString();
    return 1;
  }
#endif

  QApplication a( argc, argv );

  //PointCollection* p = createPoints( file );

  PointCollection* p = createRandomPoints( 100 );

  MainWindow w;
  w.setCollection( p );

  w.show();

  return a.exec();
}
