/*  MainWindow.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  Copyright (c) 2014-2021 Australian Synchrotron
 *
 *  The EPICS QT Framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The EPICS QT Framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with the EPICS QT Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

/*
 * Simple GUI application to demonstrate programatically adding QE widgets to a user interface
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QELabel.h"
#include "QEComboBox.h"
#include "QEPushButton.h"
#include <QPalette>
#include "QELineEdit.h"
#include "QDialog"

// .ui file opened by push buttons and loaded into a dialog
#define DIALOG_UI ":/ui/QEWidgetDisplayDialog.ui"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Define application wide macro substitutions.
    applicationMacros = "APPL=demo";

    // Set up the environment that will be applied to all widgets created
    // from here on.
    // In this case this application is set up as the object that will respond to
    // 'new window' requests, the application wide macro substitutions are defined.
    // The details are published and any widgets created from now on will take these
    // setting as their own local environment as they are each created.
    // The profile will remain published unti releaseProfile() is called, below.
    ContainerProfile profile;
    profile.setupProfile( this, QStringList(), "", applicationMacros );

    // Read the UI
    ui->setupUi(this);

    // Create the demonstration QE widgets.
    createQELabel();
    createQEComboBox();
    createQELineEdit();
    createQPushButton(); // Not a QE widget, but click signal generates QE framework activity
    createQEPushButton();

    // Release the profile now all QE framework widgets have been created.
    profile.releaseProfile();
}

// Application wind-up
MainWindow::~MainWindow()
{
    delete ui;
}


// Create QELabel widgets in various flavours.
void MainWindow::createQELabel()
{
    // Create a QELabel
    QELabel* qel = new QELabel( centralWidget() );
//    QELabel* qel = new QELabel( "OOE:ai", centralWidget() );  // See below for all the alternatives for setting a variable name

    // Set its position and size
    qel->setGeometry( 100, 30, 200, 30 );

    // Display alarm state option 1.
    // Don't display the alarm state. This is done by setting the background colour and
    // we will be setting the background ourselves and don't want it overwritten
//    qel->setDisplayAlarmStateOption( standardProperties::DISPLAY_ALARM_STATE_NEVER );

    // Display alarm state option 2.
    // Don't display the alarm state unless it is actually in alarm.
    // This is done by setting the background colour and
    // we will be setting the background ourselves and don't want it overwritten unless nessesary
    qel->setDisplayAlarmStateOption( standardProperties::DISPLAY_ALARM_STATE_WHEN_IN_ALARM );

    // Display alarm state option 3.
    // Always display display the alarm state even when the current alarm state is 'No Alarm'.
    // This is done by setting the background colour which will make setting the background ourselves
    // (below) redundant.
//    qel->setDisplayAlarmStateOption( standardProperties::DISPLAY_ALARM_STATE_ALWAYS );

    // Set the background to blue
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::blue );
    qel->setAutoFillBackground(true);
    qel->setPalette(Pal);

    //=============================================================================================
    // Set the variable name and establish a CA connection using one of the following alternatives:
    // Only one of the following alternatives should be uncommented.
    //=============================================================================================

    // Alternative 1.
    // Set the variable name using the access function for the variable name properties.
    // For a widget with only a single variable this is generally setVariableNameProperty().
    // This will activate the widget using the variable name and the current macro substitutions.
    // Note, there is no variable index. QE widgets that take more than one variable name have a different
    // set and get property function for each variable. For example, QEImage has setVariableName0Property(),
    // setVariableName1Property(), etc.
    // CA connection is established for the resultant variable immedietly (unless called from within
    // designer when the connection is only established once the user has stopped typing for a short time).
    // Using setVariableNameProperty() and similar 'set variable name property' functions has the advantage
    // that only a single call is required to set the variable name and to establish the connection.
    // Macro substitutions should be set beforehand if required using setVariableNameSubstitutionsProperty().
    // Calling setVariableNameSubstitutionsProperty() later works but an inapropriate connection
    // without macro substitutions will be attempted first.

/*
    qel->setVariableNameSubstitutionsProperty( "P=OOE" );
    qel->setVariableNameProperty( "$(P):ai" );
*/


    //=============================================================================================
    // Alterntive 2.
    // Set the variable name and substitutions.
    // CA connection is established for the resultant variable immedietly.
    // Using this alternative has the advantage of setting the variable name and macro substitutions,
    // and establishing a CA connection, in a single call.

    qel->setVariableNameAndSubstitutions( "$(P):angle", "P=ctrluser:circle", 0 );



    //=============================================================================================
    // Alternative 3.
    // Set the variable name and substitutions seperately, then once variable names are resolved
    // establish a CA connection using activate().
    // Note, the calls to setVariableNameSubstitutions() and setVariableName() can occur in any order and
    // setVariableNameSubstitutions() is optional.
    // Using this alternative has the advantage of seperating the tasks of dealing with macro substitutions, variable names, and establisghing CA connections.
    // Note, calling activate() does more than just establish a CA connection, it also asks the widget
    // to perform any tasks which should only be done once all other widgets have been created. For example,
    // calling activate() may notify other widgets about itself so activate() should only be called once all other widgets this widget may relate to are constructed.

/*
    qel->setVariableNameSubstitutions( "P=OOE" );
    qel->setVariableName( "$(P):ai", 0 );
    qel->activate();
*/

    //=============================================================================================
    // Alternative 4.
    // !!!!! CURRENTLY this alternative is unavailable as establishConnection() is not public. Should this change?
    // Set the variable name and substitutions seperately, then once variable names are resolved
    // establish a CA connection using establishConnection().
    // Note, the calls to setVariableNameSubstitutions() and setVariableName() can occur in any order and
    // setVariableNameSubstitutions() is optional.
    // Using this alternative has the advantage of seperating the tasks of dealing with macro substitutions, variable names, and establisghing CA connections.
    // Note, unlike activate() in alternative 3 calling establishConnection() only updates the
    // CA connection.

/*
    qel->setVariableNameSubstitutions( "P=OOE" );
    qel->setVariableName( "$(P):ai", 0 );
    qel->establishConnection( 0 );
*/

    //=============================================================================================
    // Alternative 4.
    // Don't uncomment this here! If you want tp try it out, uncomment it above.
    // It is a different way to construct the label and is duplicated here for the sake of the narative.
    // When this constructor is used, the variable name is set and the widget is activated immedietly.

//    QELabel* qel = new QELabel( "OOE:ai", centralWidget() );


    //=============================================================================================
    // Alternative 5.
    // In this simplistic example this alternative appears cumbersome.
    // This alternative is used when setting the properties is out of the control of the programmer.
    // For example, when loading a .ui file using Qt's UI loader.
    // When using the UI loader, the caller has no clue what widgets are being created,
    // what properties are being set, in what order, and when all properties have been set.
    // This alternative allows the establishment of a CA connection to be held off until it is
    // cirtain that all property set functions that are going to be called have been called.
    // QEForm uses this alternative when loading .ui files.

    // Flag we don't want immediate activation...
/*
    ContainerProfile profile;
    bool oldDontActivateYet = profile.setDontActivateYet( true );

    // Set the properties. Here we know exactly what we are setting, but when loading a .ui
    // file using Qt's UI loader, we don't know what properties are being set, or in what order...
    qel->setVariableNameProperty( "$(P):ai" );
    qel->setVariableNameSubstitutionsProperty( "P=OOE" );

    // Remove the flag and establish a CA connection...
    profile.setDontActivateYet( oldDontActivateYet );
    qel->activate();
*/
}

// Create a QEComboBox
void MainWindow::createQEComboBox()
{
    // Create a QEComboBox and set its position and size
    QEComboBox* qecb = new QEComboBox( centralWidget() );
    qecb->setGeometry( 100, 90, 200, 30 );

    // Don't display the alarm state. This is done by setting the background colour and
    // we will be setting the background ourselves and don't want it overwritten
    qecb->setDisplayAlarmStateOption ( standardProperties::DISPLAY_ALARM_STATE_NEVER );

    // Set the variable name using the access function for the variable name property.
    // This will activate the widget using the variable name and the current macro substitutions.
    // (See the creation of the QELabel above to see alternative ways of setting the
    // variable name and macro substitutions).
    qecb->setVariableNameProperty( "OOE:mbbi" );
}

// Create a QELineEdit
void MainWindow::createQELineEdit()
{
    // Create a QELineEdit with an active variable
    // and set its position and size
    QELineEdit* qele = new QELineEdit( "ctrluser:circle:period", centralWidget() );
    qele->setGeometry( 100, 150, 200, 30 );
}

// Create a QPushButton that opens a .ui file as a dialog box
void MainWindow::createQPushButton()
{
    // Create a QPushButton that opens a .ui file as a dialog box
    // All the QE framework tasks are performed in the button's click slot buttonClick()
    QPushButton* qpb = new QPushButton( centralWidget() );
    qpb->setGeometry( 100, 200, 200, 30 );
    qpb->setText( "Open a dialog from a QPushButton" );
    QObject::connect( qpb, SIGNAL (clicked(bool)), this, SLOT (buttonClick(bool)) );

    // A QPushButton is also defined in the main window .ui file.
    // Link that push button to the same slot as the push button above.
    QObject::connect( ui->pushButton, SIGNAL (clicked(bool)), this, SLOT (buttonClick(bool)) );
}

// Create a QEPushButton that opens a .ui file as a dialog box.
void MainWindow::createQEPushButton()
{
    // Create a QEPushButton that opens a .ui file as a dialog box.
    // This uses the built in QE framework mechanism where the QE push button
    // makes a request present a window. In this case, this main window class
    // was set up in the ContainerProfile class as the object that would respond
    // that request.
    // When the QE push button is clicked the QE framework signals the requestAction() slot
    // of this main window class with the request to open a new window.
    // Note, the QEGui application is the main example of an application that
    // responds to QE widget application requests.
    QEPushButton* qepb = new QEPushButton( centralWidget() );
    qepb->setText( "Open a dialog from a QEPushButton" );
    qepb->setGeometry( 100, 250, 200, 30 );
    qepb->setGuiName( DIALOG_UI );
    qepb->setVariableNameSubstitutions( "BTN=QE push button" );

    // A QEPushButton is also defined in the main window .ui file.
    // It will post requests just like the QEPushButton above and those
    // requests will be handled in the same way.
}

// Slot to receive a click signal from a push button and
// open a dialog.
// This slot sets up the ContainerProfile with the required macro substitutions
// then uses a QEForm to load the contents of the dialog from a .ui file.
void MainWindow::buttonClick( bool )
{
    // Set up the ContainerProfile with the required macro substitutions
    ContainerProfile profile;
    profile.setupProfile( this, QStringList(), "", "BTN=push button" );
    profile.addMacroSubstitutions( applicationMacros );

    // Build the gui and load it into a dialog.
    // Note, this is very similar to the default method that QE push buttons uses
    // to present a gui if the application has not provided a handler to
    // create GUIs through the ContainerProfile.
    QDialog* d = new QDialog;
    QEForm* gui = new QEForm( DIALOG_UI );
    if( gui )
    {
        if( gui->readUiFile())
        {
            gui->setParent( d );
            d->exec();
        }
        else
        {
            delete gui;
            gui = NULL;
        }
    }
    else
    {
        delete d;
    }

    profile.releaseProfile();
}

// Slot to receive a request to create a window from the QE framework.
// When a QE Push Button has been set up with a GUI file name and it is clicked
// it looks to see if a handler to take window creation requests has been set up
// in the ContainerProfile. This application has set up this slot as a window
// creationhandler.
// This handler does not have to be aware of any context of the widget that
// made the request. All details such as the macro substitutions required are
// provided in the request.
// open a dialog.
// For this application, only one type of request is accepted - open a .ui file.
// It uses a QEForm to load the .ui file and place it in a dialog.
void MainWindow::requestAction( const QEActionRequests& request )
{
    // Only handle file open requests
    if( request.getKind() != QEActionRequests::KindOpenFile )
    {
        return;
    }

    // If there is enough arguments, open the file
    if (request.getArguments().count () >= 1)
    {
        // Build the gui and load it into a dialog.
        // Note, this is very similar to the default method that QE push buttons uses
        // to present a gui if the application has not provided a handler to
        // create GUIs through the ContainerProfile.
        QDialog* d = new QDialog();
        QEForm* gui = new QEForm( request.getArguments().first() );
        if( gui )
        {
            if( gui->readUiFile())
            {
                gui->setParent( d );
                d->exec();
            }
            else
            {
                delete gui;
                gui = NULL;
            }
        }
        else
        {
            delete d;
        }
    }
}
