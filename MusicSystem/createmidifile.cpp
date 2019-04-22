#include "MidiFile.h"
#include <iostream>

using namespace std;
using namespace smf;


int main(int argc, char** argv) {
   MidiFile outputfile;
   outputfile.absoluteTicks(); 
   vector<uchar> midievent; 
   midievent.resize(3);
   int tpq = 48;
   outputfile.setTicksPerQuarterNote(tpq);
   outputfile.addTrack(1);

   // data to write to MIDI file: (60 = middle C)
   // C5 C  G G A A G-  F F  E  E  D D C-
   int melody[50]  = {72,72,79,79,81,81,79,77,77,76,76,74,74,72,-1};
   int mrhythm[50] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2,-1};

   // C3 C4 E C F C E C D B3 C4 A3 F G C-
   int bass[50] =   {48,60,64,60,65,60,64,60,62,59,60,57,53,55,48,-1};
   int brhythm[50]= { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,-1};


   int i=0;
   int actiontime = 0;
   midievent[2] = 64;
   while (melody[i] >= 0) {
      midievent[0] = 0x90;
      midievent[1] = melody[i];
      outputfile.addEvent(0, actiontime, midievent);
      actiontime += tpq * mrhythm[i];
      midievent[0] = 0x80;
      outputfile.addEvent(0, actiontime, midievent);
      i++;
   }
   i = 0;
   actiontime = 0;
   while (bass[i] >= 0) {
	   midievent[0] = 0x90;
	   midievent[1] = bass[i];
	   outputfile.addEvent(1, actiontime, midievent);
	   actiontime += tpq * brhythm[i];
	   midievent[0] = 0x80;
	   outputfile.addEvent(1, actiontime, midievent);
	   i++;
   }

   outputfile.sortTracks();
   outputfile.write("twinkle.mid");
   return 0;
}