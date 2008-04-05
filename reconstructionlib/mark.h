/****************************************************************************/
/*                                                                          */
/* File:      mark.h                                                        */
/*                                                                          */
/* Purpose:   datastructure for communcication and modification of marksr   */
/*                                                                          */
/* Author:    Philip Broser                                                 */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     philip.broser@iwr.uni-heidelberg.de                           */
/*                                                                          */
/* History:   22.06.03 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/

#ifndef MARK_H
#define MARK_H
#include "../datalib/vector.h"

#define LABELLENGTH 40

/*!
* Class to handle exchange of marks
 */

//inaktive : initalisation, aktive : new set mark , linked : mark beloning to a compartment, deleted : deleted mark
enum State{inaktive, aktive, linked, deleted};

//Types of marks
enum typesOfMarks {TerminationType, LinkType, OrientationType};

//Which kind of orientation Mark: 
enum kindsOfOrientationMarks {BarrelCenter, BarrelDirection, Pia, Arbitrary};

//Rows and Colums (only for Barrel marks)
enum BarrelRow {A, B, C, D, E, F, G, H};
typedef int BarrelColumn;

class Mark
{
public:
    Mark();
    Mark(Vector<int> &pos, State st, int idOfT);
    int operator=(Mark &lo);
    int GetId() {return id;};
    int GetIdOfTermination() {return idOfTermination;};
    int SetIdOfTermination(int i) {idOfTermination=i;return 0;}
    int SetPosition(Vector<int> pos) {position=pos;return 0;}
    Vector<int> GetPosition() {return position;}
    int SetType(typesOfMarks typeOfM) {typeOfMark=typeOfM;return 0;};
    int SetKindOfOrientation(kindsOfOrientationMarks kindOfO) {kindOfMark= kindOfO;return 0;};
    typesOfMarks GetType() {return  typeOfMark;};
    kindsOfOrientationMarks GetKind() {return kindOfMark;};
    
    bool IsLableEditable() {return  labelIsEditable;};
    int GetMarkLabel(char presentLabel[LABELLENGTH]); // should be fiel of type  char[LABELLENGTH]
    int SetMarkLabel(char newLabel[LABELLENGTH]); // should be fiel of type  char[LABELLENGTH]
    
private:
    bool labelIsEditable;
    char label[LABELLENGTH];
    State state;
    typesOfMarks typeOfMark; //default is Termination
    kindsOfOrientationMarks kindOfMark; //which kind of orientation mark
    
    
    int idOfTermination; //<- Hi, Alexander das ist die Entscheidende ID fŸr dich!!! 
    Vector<int> position;
    int id;	// <- nur fŸr Verwaltung innherhalb meiner sourcen
    static int idCounter;
};







#endif
