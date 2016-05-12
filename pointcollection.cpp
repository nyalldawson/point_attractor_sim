#include "pointcollection.h"
#include "datapoint.h"
#include <QGraphicsRectItem>
#include <QtGlobal>
#include <QTextStream>
#include <QGraphicsLineItem>

#define MAX_POINTS 10000
#define DEFAULT_RANDOM_COUNT   10
#define DEFAULT_RANDOM_HUE_MIN 0
#define DEFAULT_RANDOM_HUE_MAX 359
#define DEFAULT_RANDOM_VAL_MIN 200
#define DEFAULT_RANDOM_VAL_MAX 240
#define DEFAULT_RANDOM_SAT_MIN 100
#define DEFAULT_RANDOM_SAT_MAX 240

PointCollection::PointCollection()
  : QGraphicsScene()
  , mCutoff( 1.144252 )
  , mDistanceDecay( 0.0 )
  , mIteration( 0 )
{
  setSceneRect( 0, 0, 500, 500 );
  // draw bounds of simulation
  addEllipse( 0, 0, 500, 500, QPen( QColor( 200, 200, 200 ) ), Qt::NoBrush );
}

inline int PointCollection::hashId( int id1, int id2 ) const
{
  int lowId = qMin( id1, id2 );
  int highId = qMax( id1, id2 );
  return lowId * MAX_POINTS + highId;
}

void PointCollection::setAttraction( int id1, int id2, double attraction )
{
  mAttractionHash.insert( hashId( id1, id2 ), attraction );
}

double PointCollection::attraction( int id1, int id2 ) const
{
  int id = hashId( id1, id2 );
  if ( !mAttractionHash.contains( id ) )
    return 0.0;

  return mAttractionHash.value( id ) - mCutoff;
}

void PointCollection::addDataPoint( DataPoint* point )
{
  mDataPoints.insert( point->id(), point );
  QGraphicsLineItem* line = new QGraphicsLineItem();
  line->setPen( QPen( QColor( 220, 220, 220 ) ) );
  mLines.append( line );
  point->setZValue( 10 );
  addItem( point );
  line->setZValue( 0 );
  addItem( line );
}

DataPoint* PointCollection::dataPoint( int id ) const
{
  if ( !mDataPoints.contains( id ) )
    return 0;

  return mDataPoints.value( id );
}

void PointCollection::placeRandom()
{
  PointHash::const_iterator pointIt = mDataPoints.constBegin();
  for ( ; pointIt != mDataPoints.constEnd(); ++pointIt )
  {
    double angle = 360.0 * ( double )qrand() / RAND_MAX ;
    double length = sceneRect().width() / 2.0 * sqrt( ( double )qrand() / RAND_MAX  );
    QLineF line( sceneRect().width() / 2.0, sceneRect().height() / 2.0, 0 , 0 );
    line.setAngle( angle );
    line.setLength( length );
    pointIt.value()->setPos( line.p2() );
  }
  mIteration = 0;
}

void PointCollection::assignColors()
{
  int colorCount = mDataPoints.count();

  double hueOffset = ( 360.0 * qrand() / ( RAND_MAX + 1.0 ) );

  //try to maximise difference between hues. this is not an ideal implementation, as constant steps
  //through the hue wheel are not visually perceived as constant changes in hue
  //(for instance, we are much more likely to get green hues than yellow hues)
  double hueStep = 359.0 / colorCount;
  double currentHue = hueOffset;

  //build up a list of colors
  QList<QColor> colors;
  for ( int idx = 0; idx < colorCount; ++ idx )
  {
    int h = qRound( currentHue ) % 360;
    int s = ( qrand() % ( DEFAULT_RANDOM_SAT_MAX - DEFAULT_RANDOM_SAT_MIN + 1 ) ) + DEFAULT_RANDOM_SAT_MIN;
    int v = ( qrand() % ( DEFAULT_RANDOM_VAL_MAX - DEFAULT_RANDOM_VAL_MIN + 1 ) ) + DEFAULT_RANDOM_VAL_MIN;
    colors.append( QColor::fromHsv( h, s, v ) );
    currentHue += hueStep;
  }

  int i = 0;
  PointHash::const_iterator pointIt = mDataPoints.constBegin();
  for ( ; pointIt != mDataPoints.constEnd(); ++pointIt )
  {
    pointIt.value()->setColor( colors.at( i ) );
    ++i;
  }
}

void PointCollection::doStep()
{
  PointHash::const_iterator pointIt1 = mDataPoints.constBegin();
  for ( ; pointIt1 != mDataPoints.constEnd(); ++pointIt1 )
  {
    PointHash::const_iterator pointIt2 = mDataPoints.constBegin();
    for ( ; pointIt2 != mDataPoints.constEnd(); ++pointIt2 )
    {
      if ( pointIt1.value()->id() == pointIt2.value()->id() )
        continue;
      pointIt1.value()->calcDelta( pointIt2.value(),
                                   attraction( pointIt1.value()->id(), pointIt2.value()->id() ), mDistanceDecay );
    }
  }

  PointHash::const_iterator pointIt = mDataPoints.constBegin();
  for ( ; pointIt != mDataPoints.constEnd(); ++pointIt )
  {
    pointIt.value()->doStep();
  }

  drawLines();

  mIteration++;
}

void PointCollection::drawLines()
{
  int i = 0;
  PointHash::const_iterator pointIt1 = mDataPoints.constBegin();
  for ( ; pointIt1 != mDataPoints.constEnd(); ++pointIt1 )
  {
    //p1->doStep();

    double minDist = DBL_MAX;
    DataPoint* closestNode = 0;
    QLineF closestLine;
    PointHash::const_iterator pointIt2 = mDataPoints.constBegin();
    for ( ; pointIt2 != mDataPoints.constEnd(); ++pointIt2 )
    {
      if ( pointIt1.value()->id() == pointIt2.value()->id() )
        continue;

      QLineF line( pointIt1.value()->pos(), pointIt2.value()->pos() );
      if ( line.length() < minDist )
      {
        minDist = line.length();
        closestLine = line;
      }
    }
    if ( closestNode >= 0 )
    {
      mLines.at( i )->setLine( closestLine );
    }
    ++i;
  }
}

void PointCollection::bump()
{
  PointHash::const_iterator pointIt = mDataPoints.constBegin();
  for ( ; pointIt != mDataPoints.constEnd(); ++pointIt )
  {
    pointIt.value()->bump();
  }
}

int PointCollection::iteration() const
{
  return mIteration;
}

void PointCollection::exportLocations( QFile& file )
{
  file.open( QIODevice::WriteOnly | QIODevice::Text );

  QTextStream out( &file );
  out << "id,x,y\n";
  PointHash::const_iterator pointIt = mDataPoints.constBegin();
  for ( ; pointIt != mDataPoints.constEnd(); ++pointIt )
  {
    out << QString( "%1,%2,%3" ).arg( pointIt.value()->id() )
        .arg( pointIt.value()->pos().x() )
        .arg( pointIt.value()->pos().y() ) << "\n";
  }

  file.close();
}
