#include "Brain.h"

void Neuron::DrawBoundingBox(bool do_shift){

    glPushMatrix();

    if(do_shift == true){
        glTranslatef(center_[0],center_[1],center_[2]);
    }

    glBegin(GL_LINES);

    glVertex3f( max_[0] , max_[1] , max_[2] );
    glVertex3f( min_[0] , max_[1] , max_[2] );

    glVertex3f( max_[0] , min_[1] , max_[2] );
    glVertex3f( min_[0] , min_[1] , max_[2] );

    glVertex3f( max_[0] , min_[1] , min_[2] );
    glVertex3f( min_[0] , min_[1] , min_[2] );

    glVertex3f( max_[0] , max_[1] , min_[2] );
    glVertex3f( min_[0] , max_[1] , min_[2] );

    glVertex3f( max_[0] , max_[1] , max_[2] );
    glVertex3f( max_[0] , min_[1] , max_[2] );

    glVertex3f( max_[0] , max_[1] , min_[2] );
    glVertex3f( max_[0] , min_[1] , min_[2] );

    glVertex3f( max_[0] , max_[1] , max_[2] );
    glVertex3f( max_[0] , max_[1] , min_[2] );

    glVertex3f( max_[0] , min_[1] , max_[2] );
    glVertex3f( max_[0] , min_[1] , min_[2] );

    glVertex3f( min_[0] , max_[1] , max_[2] );
    glVertex3f( min_[0] , min_[1] , max_[2] );

    glVertex3f( min_[0] , max_[1] , min_[2] );
    glVertex3f( min_[0] , min_[1] , min_[2] );

    glVertex3f( min_[0] , max_[1] , max_[2] );
    glVertex3f( min_[0] , max_[1] , min_[2] );

    glVertex3f( min_[0] , min_[1] , max_[2] );
    glVertex3f( min_[0] , min_[1] , min_[2] );

    glEnd();

    glPopMatrix();

}
