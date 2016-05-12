#ifndef POINTCOLLECTION_H
#define POINTCOLLECTION_H

#include "datapoint.h"
#include <QGraphicsScene>
#include <QHash>
#include <QFile>

class QGraphicsLineItem;

/** Collection of points displayed in a graphics scene
 */
class PointCollection : public QGraphicsScene
{
  public:

    PointCollection();

    //! Sets the attraction cut off - the point at which attraction becomes
    //! repulsion
    void setCutoff( double cutoff )
    {
      mCutoff = cutoff;
    }

    //! Sets a distance decay factor for the attraction between two distant points
    void setDistanceDecay( double decay )
    {
      mDistanceDecay = decay;
    }

    //! Returns the distance decay factor
    double distanceDecay() const
    {
      return mDistanceDecay;
    }

    /** Sets the attraction factor between two points
     * @param id1 point 1 ID
     * @param id2 point 2 ID
     * @param attraction attraction factor
     * @see attraction()
     */
    void setAttraction( int id1, int id2, double attraction );

    /** Returns the attraction factor between two points
     * @param id1 point 1 ID
     * @param id2 point 2 ID
     * @see setAttraction()
     */
    double attraction( int id1, int id2 ) const;

    /** Adds a data point to the simulation.
     * @param point data point, ownership is transferred
     * @see dataPoint()
     */
    void addDataPoint( DataPoint* point );

    /** Returns a pointer to the data point with a specified ID
     * @param id ID of data point
     */
    DataPoint* dataPoint( int id ) const;

    //! Randomly initialises all point locations
    void placeRandom();

    //! Assigns random colors to points
    void assignColors();

    //! Runs a simulation for a single frame
    void doStep();

    //! Draws lines between each point and its nearest neighbour
    void drawLines();

    //! Bumps points - adds a random offset to their location to add entropy to the simulation
    void bump();

    //! Returns the current number of iterations elapsed
    int iteration() const;

    //! Exports the current location of points to a file
    void exportLocations( QFile& file );

  protected:

    //! Hash of attraction between points
    //! @see hashId()
    QHash<int, double> mAttractionHash;

    typedef QHash<int, DataPoint* > PointHash;

    //! Hash of point ID to data points
    PointHash mDataPoints;

    //! Threshold at which attraction becomes repulsion
    double mCutoff;
    //! Distance decay factor
    double mDistanceDecay;
    //! Iteration number
    int mIteration;

    //! Nearest neighbour lines
    QList<QGraphicsLineItem*> mLines;

    //! Calculates a unique hash ID for two points
    inline int hashId( int id1, int id2 ) const;
};

#endif // POINTCOLLECTION_H
