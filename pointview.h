#ifndef POINTVIEW_H
#define POINTVIEW_H

#include <QGraphicsView>

/** QGraphicsView subclass which allows zooming with mouse wheel
 */
class PointView : public QGraphicsView
{
  public:

    /** Constructor for PointView
     * @param parent parent widget
     */
    PointView( QWidget* parent = NULL );

  protected:

    //Take over the interaction
    virtual void wheelEvent( QWheelEvent* event );
};

#endif // POINTVIEW_H
