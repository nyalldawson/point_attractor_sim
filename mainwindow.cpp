#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pointcollection.h"

#include <QSettings>
#include <QTimer>

MainWindow::MainWindow( QWidget* parent )
  : QMainWindow( parent )
  , ui( new Ui::MainWindow )
  , mCollection( 0 )
{
  ui->setupUi( this );

  QSettings settings( "nd", "gravity" );
  restoreGeometry( settings.value( "window/geometry" ).toByteArray() );
  restoreState( settings.value( "window/windowState" ).toByteArray() );

  mTimer.setInterval( 1000 / 60.0 );
  connect( &mTimer, SIGNAL( timeout() ), this, SLOT( doStep() ) );
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::setCollection( PointCollection* collection )
{
  ui->mGraphicsView->setScene( collection );
  ui->mGraphicsView->setSceneRect( collection->sceneRect() );
  mCollection = collection;
}

void MainWindow::closeEvent( QCloseEvent* event )
{
  QSettings settings( "nd", "gravity" );
  settings.setValue( "window/geometry", saveGeometry() );
  settings.setValue( "window/windowState", saveState() );
  QMainWindow::closeEvent( event );
}

void MainWindow::on_mReplaceButton_clicked()
{
  mCollection->placeRandom();
}

void MainWindow::on_mStartButton_clicked()
{
  if ( !mTimer.isActive() )
  {
    mTimer.start();
    ui->mStartButton->setText( "Pause" );
  }
  else
  {
    mTimer.stop();
    ui->mStartButton->setText( "Play" );
  }
}

void MainWindow::on_mBumpButton_clicked()
{
  mCollection->bump();
}

void MainWindow::on_mExportButton_clicked()
{
  QFile file( "d:/result.csv" );
  mCollection->exportLocations( file );
}

void MainWindow::on_mCutoffSpinBox_valueChanged( double val )
{
  mCollection->setCutoff( val );
}

void MainWindow::on_mFrameRateSpin_valueChanged( int val )
{
  mTimer.setInterval( 1000.0 / val );
}

void MainWindow::on_mDistanceDecaySpinBox_valueChanged( double val )
{
  mCollection->setDistanceDecay( val );
}

void MainWindow::doStep()
{
  if ( !mCollection )
    return;

  mCollection->doStep();
  ui->mStatusBar->showMessage( QString( "Iteration: %1" ).arg( mCollection->iteration() ) );
}
