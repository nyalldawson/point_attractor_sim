#include "datapoint.h"
#include <QBrush>
#include <QPen>
#include <QCursor>
#include <QGraphicsScene>
#include <QRectF>

DataPoint::DataPoint( int id )
  : QGraphicsEllipseItem()
  , mId( id )
{
  setRect( -4, -4, 8, 8 );
  setBrush( QBrush( Qt::red ) );
  setPen( Qt::NoPen );
  setFlag( QGraphicsItem::ItemIsMovable, true );
  setCursor(  Qt::ArrowCursor );

  setToolTip( QString::number( id ) );
}

void DataPoint::setColor( const QColor& color )
{
  setBrush( QBrush( color ) );
}

void DataPoint::calcDelta( const DataPoint* p2, double attraction, double distanceDecay )
{
  QLineF line = QLineF( pos(), p2->pos() );
  double distance = line.length();

  double step = 0.01 * force( distance, attraction, distanceDecay );
  if ( step > distance )
  {
    step = distance;
  }

  line.setLength( step );

  mDelta.rx() += line.dx();
  mDelta.ry() += line.dy();
}

void DataPoint::doStep()
{
  setPos( checkBounds( QPointF( pos().x() + mDelta.x(), pos().y() + mDelta.y() ) ) );
  mDelta.setX( 0 );
  mDelta.setY( 0 );
}

void DataPoint::bump()
{
  double bumpStep = 30;
  double x = pos().x() + bumpStep * ( double )qrand() / RAND_MAX - bumpStep / 2.0;
  double y =  pos().y() +  bumpStep * ( double )qrand() / RAND_MAX - bumpStep / 2.0;
  setPos( checkBounds( QPointF( x, y ) ) );
}

QPointF DataPoint::checkBounds( const QPointF& pos ) const
{
  double centerX = scene()->sceneRect().width() / 2.0;
  double centerY = scene()->sceneRect().height() / 2.0;
  QLineF line( QPointF( centerX, centerY ), pos );
  double radius = centerX;
  if ( line.length() > radius )
  {
    line.setLength( radius );
  }
  return line.p2();
}

double DataPoint::force( double distance, double attraction, double distanceDecay ) const
{
  return ( distance < 30 ? ( -100 / distance ) : distanceDecay * distance ) + attraction;
}
