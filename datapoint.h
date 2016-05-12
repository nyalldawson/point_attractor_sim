#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <QGraphicsEllipseItem>

/** Item representing a single data point
 */
class DataPoint : public QGraphicsEllipseItem
{
  public:

    /** Constructor for a data point
     * @param id point identifier
     */
    DataPoint( int id );

    //! Returns the point's ID
    int id() const
    {
      return mId;
    }

    /** Sets a color for the point
     * @param color point fill color
     */
    void setColor( const QColor& color );

    /** Calculates the point's displacement as a result of the attraction to another
     * point.
     * @param p2 secondary point to be attracted to
     * @param attraction attraction factor
     * @param distanceDecay distance decay factor
     * @see doStep()
     */
    void calcDelta( const DataPoint* p2, double attraction, double distanceDecay );

    /** Moves the point to its final position after all deltas have been calculated
     * @see calcDelta()
     * @see bump()
     */
    void doStep();

    /** Bumps the point - moves it by a random offset to add "noise" into the system
     */
    void bump();

  private:

    //! ID of point
    int mId;

    //! Current calculated point offset since last step
    QPointF mDelta;

    /** Calculates the force of attraction (or repulsion) for a given distance
     * @param distance distance between points
     * @param attraction attraction factor
     * @param distanceDecay distance decay factor
     */
    double force( double distance, double attraction, double distanceDecay ) const;

    /** Restricts the point to within circular bounds, by moving it to the closest point
     * within the circle. Circle dimensions match the graphics scene dimensions
     * @param pos initial position
     * @returns closest position within circle
     */
    QPointF checkBounds( const QPointF& pos ) const;
};

#endif // DATAPOINT_H
