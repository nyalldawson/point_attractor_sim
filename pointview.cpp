#include "pointview.h"
#include <QWheelEvent>

PointView::PointView( QWidget* parent )
  : QGraphicsView( parent )
{
  setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform );
  //setSceneRect(0, 0, 20, 20);
  setDragMode( ScrollHandDrag );
}

void PointView::wheelEvent( QWheelEvent* event )
{
  setTransformationAnchor( QGraphicsView::AnchorUnderMouse );

  // Scale the view / do the zoom
  double scaleFactor = 1.15;
  if ( event->delta() > 0 )
  {
    // Zoom in
    scale( scaleFactor, scaleFactor );
  }
  else
  {
    // Zooming out
    scale( 1.0 / scaleFactor, 1.0 / scaleFactor );
  }

  // Don't call superclass handler here
  // as wheel is normally used for moving scrollbars
}
