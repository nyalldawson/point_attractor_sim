#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

class PointCollection;

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow( QWidget* parent = 0 );
    ~MainWindow();

    /** Sets the point collection associated with the display
     * @param collection point collection, ownership is not transferred
     */
    void setCollection( PointCollection* collection );

  public slots:

    void on_mReplaceButton_clicked();
    void on_mStartButton_clicked();
    void on_mBumpButton_clicked();
    void on_mExportButton_clicked();
    void on_mCutoffSpinBox_valueChanged( double val );
    void on_mFrameRateSpin_valueChanged( int val );
    void on_mDistanceDecaySpinBox_valueChanged( double val );

    void doStep();

  protected:
    void closeEvent( QCloseEvent* event );

  private:
    Ui::MainWindow* ui;

    PointCollection* mCollection;
    QTimer mTimer;
};

#endif // MAINWINDOW_H
