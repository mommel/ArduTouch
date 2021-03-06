//
//  Dronetic.ino
//
//  A droning synth using massed quantum square wave oscillators. 
//
//  To get the most out of Dronetic please read these comments first.
//
//  For an audio demo of Dronetic go to https://www.youtube.com/c/mitchaltman23
//
//  Target:   ArduTouch board
// 
//   -----------------------------------------------------------------------
//
//              ==== Audio Flow Diagram for the Dronetic Synth  ====
//
//
//          -------------      -----------------        -------------
//          |  Drone 0  | ---> | Stereo Panner |  <---  |  Drone 1  |
//          -------------      -----------------        -------------
//                                   |   |
//                                   |   |
//                                   v   v
//                                -----------
//            [left audio]  <---  |  Mixer  |  ---> [right audio]
//                                -----------
//                                     ^
//                                     |
//                                     |
//                              ----------------
//                              |  Lead Voice  |
//                              ----------------
//
//   -----------------------------------------------------------------------
//         
//                      ==== Diagram of the Drone Voice ====
//
//
//        ----------------------                 
//        | Oscillator Section | --> AutoWah --> Gain --> [output]
//        ----------------------                                           
//              
//   -----------------------------------------------------------------------
//              
//               ==== Diagram of the Drone Oscillator Section ====
//              
//          -----------------------------------------
//          |                                       |
//          |     ---------                         |
//          |  -->| Osc 0 |     all oscillators are |
//          |  |  ---------     square waves with   |
//          |  |                integral periods.   |       
//          |  |  ---------                         |
//          |  -->| Osc 1 |     each osc can be     |
//   notes  |  |  ---------     switched on or off. |   -----> [output]
//   -----> |  |                                    |
//   route  |  |  ---------     output is combined. |
//    to    |  -->| Osc 2 |                         |
//  current |  |  ---------           ---------------
//    Osc   |  |                      |  Transpose  |  # semi-tones to transpose note 
//          |  |  ---------           ---------------
//          |  -->| Osc 3 |           |   Freeze    |  ignore note input
//          |  |  ---------           ---------------   
//          |  |                      | Pulse Width |  set pulse width for all osc
//          ---|-------------------------------------
//             |
//    select current Osc (follows note input, with transposition, unless Freeze)
//              
//   -----------------------------------------------------------------------
//         
//                      ==== Diagram of the Lead Voice ====
//
//            Lead Voice comes in two varieties, Ambient and Caustic
//
//   -----------------------------------------------------------------------
//                                     |
//                Ambient              |               Caustic
//                                     |
//      -----------                    |  --------------        -------
//      | WaveOsc |  --> [output]      |  | PureSquare |  ----> |     |
//      -----------                    |  --------------    ^   |     |
//           ^                         |               freq |   |     |
//           |                         |               diff |   | XOR | --> [output]
//        "Ether"                      |  -----------       v   |     |
//       Wavetable                     |  | WaveOsc |  -------> |     |
//                                     |  -----------           |     |
//                                     |       ^                -------
//                                     |       |
//                                     |    "Ether"
//                                     |   Wavetable
//                                     |
//                                     |
//      Ambient is default voice       |   Caustic is used in 2 presets 
//                                     |
//   -----------------------------------------------------------------------
//
//                    Interaction between Lead Voice and Drones
//
//   When a note is played by the lead voice, that note is also routed to 
//   each drone. The drone will then either ignore the note (because Freeze 
//   is enabled -- see diagram for Drone Oscillator Section above) or it will 
//   play the note (transposed by the number of semi-tones specified by 
//   Transpose) using its currently selected oscillator.
//
//   Each preset pre-determines whether note input is frozen, and what the 
//   selected oscillator is for each drone. The value of Transpose can be 
//   changed via the onboard pots. See below for more details.
//
//   -----------------------------------------------------------------------
//
//  How to use:
//
//  Double-tap the left button and the drone voices will begin.
//  Double-tap the left button again and the drone voices will stop.
//
//  So, double-tapping the left button toggles the drone voices on and off.
//
//  When the drone voices are off you can also start them by pressing any key
//  (which will also be played as a note in the lead voice).
//
//  Once the drone voices have started, notes that you play on the keyboard
//  will control the lead voice (and perhaps the current osc in each drone). 
//
//  To raise the lead voice by an octave, tap the right button once.
//  To lower the lead voice by an octave, tap the left button once.
//
//  Each LED can be in one of 3 states : OFF - ON - BLINKING  
//
//  The LEDs indicate what parameters the pots control:
//
//     red LED    blue LED         top POT                  bot POT
//     -------    --------   -------------------      -------------------
//       OFF         OFF       autowah 0 FREQ           autowah 0 DEPTH
//        ON         OFF       autowah 1 FREQ           autowah 1 DEPTH
//     BLINK         OFF        panning FREQ             panning DEPTH
//       OFF          ON       autowah 0 CUTOFF        autowah 1 CUTOFF
//        ON          ON     drone 0 PULSE WIDTH      drone 1 PULSE WIDTH
//     BLINK          ON      drone 0 TRANSPOSE*       drone 1 TRANSPOSE**
//       OFF       BLINK         lead ATTACK               lead DECAY         
//        ON       BLINK         lead SUSTAIN             lead RELEASE
//     BLINK       BLINK        XOR FREQ DIFF***         lead PORTAMENTO
//
//  To bump the red  LED state: press left button
//  To bump the blue LED state: press right button
//
//  So, by pressing the left and right buttons you can change what parameters
//  are controlled by the pots.
//
//    * current osc only, effective on presets: Eire, Oslo
//   ** current osc only, effective on presets: Borea, Crystal, Eire, Faroe, Oslo
//  *** effective on presets: Cerebus Riven
//
//  ---------------------------------------------------------------------------
//                         Guide to Dronetic's Presets 
//  ---------------------------------------------------------------------------
//
//   Load presets by double-tapping the right button and then pressing any 
//   "white" key. Once a preset is loaded, the drone voices for that preset
//   will begin to play.
//
//   The presets are as follows:
//
//      Key   Name      Performance Notes   
//      ---   ----      ----------------------------------------------------
//       C    Borea     ambient. play in E minor / A 
//       D    Crystal   ambient. play in D minor / Bb major
//       E    Eire      ambient. play in C major
//       A    Faroe     ambient. play in D major
//       G    Oslo      ambient. play in C major
//       A    Riven     caustic. hold notes then slide finger for pitch bend
//       B    Cerebus   caustic.
//
//  ---------------------------------------------------------------------------
//                           About Dronetic's Tuning 
//  ---------------------------------------------------------------------------
//
//   When you start playing notes on the keyboard you will probably notice that 
//   the 'C' key plays an 'F'. And if you have a really good ear you may also 
//   notice that the interval between C# and G# is not exactly a 5th.
//
//   Why is this?
//
//   To explain the 2nd observation first, Dronetic uses Harmonic Tuning (also
//   known as Just Temperament or "Helmholtz's scale") instead of the usual
//   Equal Temperament (which is the system default). 
//
//   Harmonic Tuning is actually more "natural" than Equal Temperament (which
//   was created as a compromise so that keyboard instruments could play in
//   all keys). In Harmonic Tuning the relative frequencies of all notes 
//   compared to the tonic ('C' in our case) are rational numbers:
//
//              Unison         1/1  
//              Minor Second  25/24  
//              Major Second   9/8  
//              Minor Third    6/5  
//              Major Third    5/4   
//              Fourth         4/3  
//              Diminish 5th  45/32  
//              Fifth          3/2  
//              Minor Sixth    8/5  
//              Major Sixth    5/3  
//              Minor Seventh  9/5  
//              Major Seventh 15/8  
//              Octave         2/1  
//
//   Dronetic uses quantum square wave oscillators which can only be set to 
//   frequencies which have integral wavelengths. The frequencies produced by
//   these oscillators conform more closely to a scale in Harmonic Tuning than
//   one in Equal Temperament.
//
//   But why does the 'C' key play an 'F'?
//
//   The short answer is that on the ArduTouch, whose audio rate is ~15.6 kHz,
//   'F' (in octave 1) has a period of 360 samples. 360 is a number favored by the
//   ancients because it is divisible by a large number of integers: 1, 2, 3, 4, 
//   5, 6, 8, 9, 10, 12, 15, 18, 20, 24, etc. It so happens that by using a tonic
//   frequency whose period is 360, we are able to use quantum oscillators to
//   closely approximate the Harmonic Tuning scale over several octaves. Low 'F', 
//   so to speak, is the natural tonic frequency of the ArduTouch.*
//   
//   *actually, on the ArduTouch a waveform with a period of 360 samples generates 
//   a tone with a frequency of 43.578 Hz. In the equal temperament scale, 'F' in
//   octave 1 has a frequency of 43.654 Hz.  
//
//  ---------------------------------------------------------------------------
// 
//  Copyright (C) 2019, Cornfield Electronics, Inc.
// 
//  This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 
//  Unported License.
// 
//  To view a copy of this license, visit
//  http://creativecommons.org/licenses/by-sa/3.0/
// 
//  Created by Bill Alessi & Mitch Altman.
// 
//  ---------------------------------------------------------------------------/*

#include "ArduTouch.h"                       

// The following statement should not be messed with unless you want to compile 
// the DEVELOPER version of Droentic. The DEVELOPER version allows you to run the 
// Console. To use the DEVELOPER version, uncomment the following statement, set 
// the Runtime Model to __FULLHOST__ and comment out the defines for KEYBRD_MENUS 
// and IMPLICIT_SEQUENCER in Model.h. (keep in mind that KEYBRD_MENUS and 
// IMPLICIT_SEQUENCER may need to be defined to compile other synths!)

//#define DEVELOPER        // for developers only who want to run the Console

#ifdef DEVELOPER
 #ifdef KEYBRD_MENUS
   #error Comment out KEYBRD_MENUS in Model.h to run the Developers version
 #endif
#endif

#ifndef __STNDLONE__
 #ifndef DEVELOPER
   #error This sketch requires the __STNDLONE__ runtime model (Model.h)
 #endif
#endif

about_program( Dronetic, 0.64 )       

/******************************************************************************
 *
 *                               Presets
 *
 ******************************************************************************/

define_preset( Borea, "'S0ON4\\0n2\\1n9\\2n28\\3n33\\Ft\\`Ea<c50\\lf.125\\d96\\``g```"
                       "1ON4\\0n16\\1n21\\2n40\\3n28\\`Ea<c28\\lf.250\\d64\\``g```"
             )

define_preset( Cerebus, "'S0ON3\\0n2\\p48\\1n1\\p127\\2n3\\p133\\"
                        "Ft\\P108\\`Ea<c60\\lf2.953\\d46\\``g```"
                        "1ON4\\0n34\\p15\\1n21\\p23\\2n20\\p83\\3n28\\p22\\"
                        "Ft\\P63\\`Ea<c33\\lf.9\\d110\\``gg2\\```"
                        "2cv168\\O<r.5\\m78\\D200\\`ea100\\d200\\s0\\r200\\`f0\\`k1\\`"
             )

define_preset( Crystal, "'SPf.75\\d80\\P140\\p`2g60\\`"
                        "0ON4\\0n0\\1n22\\2n29\\3n38\\Ft\\`Ea<c29\\lf.150\\d128\\``g```"
                        "1ON3\\0n10\\1n24\\2n38\\`Ea<c27\\lf1.67\\d102\\``gg1.25\\```"
             )

define_preset( Eire, "'S0ON3\\0n0\\1n28\\2n31\\`Ea<c12\\lf1.0\\d77\\``gg1.5\\```"
                       "1ON3\\0n0\\1n12\\2n28\\`Ea<c5\\lf.5\\d51\\``gg1.5\\```"
             )

define_preset( Faroe, "'SPf.2\\d64\\P90\\p`"
                      "0ON4\\0n2\\p129\\1n14\\p36\\2n21\\p38\\3n30\\p3\\P98\\Ft\\`Ea<c22\\lf1.422\\d38\\``g```"
                      "1ON4\\0n26\\1n30\\2n40\\3n21\\P40\\`x7\\Ea<c15\\lf.93\\d19\\``g```k2\\`"
             )

define_preset( Oslo,  "'SPf2.25\\d77\\`"
                      "0ON3\\0n0\\1n28\\2P64\\`x-10\\Ea<c24\\lf.8\\d64\\``g```"
                      "1ON3\\0n38\\1n33\\2`x5\\Ea<c24\\lf4\\d46\\``g```k3\\`"
             )

define_preset( Riven, "'S0ON4\\0n27\\p11\\1n28\\p60\\2n31\\p28\\3n32\\p30\\Ft\\P11\\`Ea<c180\\lf.1\\d77\\``g```"
                       "1ON3\\0n22\\p69\\1n28\\p30\\2n38\\p25\\Ft\\P30\\`Ea<c22\\lf.2\\d64\\``gg1.2\\```"
                       "2cg30\\O<r.09\\m73\\D50\\`e~a151\\d100\\s128\\r160\\`f0\\`k1\\`"
             )

define_preset( Empty, "" )               

#ifdef DEVELOPER  // only one preset will be loaded -- take your pick ...

begin_bank( myPresets )                   

   _preset( Oslo )                        // C
   _preset( Empty )                       // C#
   _preset( Empty )                       // D
   _preset( Empty )                       // D#
   _preset( Empty )                       // E
   _preset( Empty )                       // F
   _preset( Empty )                       // F#
   _preset( Empty )                       // G
   _preset( Empty )                       // G#
   _preset( Empty )                       // A
   _preset( Empty )                       // A#
   _preset( Empty )                       // B

end_bank()

#else // normal version loads all presets

begin_bank( myPresets )                   

   _preset( Borea )                       // C
   _preset( Empty )                       // C#
   _preset( Crystal )                     // D
   _preset( Empty )                       // D#
   _preset( Eire )                        // E
   _preset( Faroe )                       // F
   _preset( Empty )                       // F#
   _preset( Oslo )                        // G
   _preset( Empty )                       // G#
   _preset( Riven )                       // A
   _preset( Empty )                       // A#
   _preset( Cerebus )                     // B

end_bank()

#endif

/******************************************************************************
 *
 *                        DroneOscSection class
 *
 ******************************************************************************/

const word NotePeriods[] PROGMEM = {  

   // pre-calculated quantum oscillator periods per note

   // octave 0

   360, 346, 320, 300, 288, 270, 256, 240, 225, 214, 200, 192,

   // octave 1

   180, 173, 160, 150, 144, 135, 128, 120, 113, 107, 100, 96,

   // octave 2
   
   90, 86, 80, 75, 72, 67, 64, 60, 56, 54, 50, 48,

   // octave 3
   
   45, 43, 40, 38, 36, 34, 32, 30, 28, 27, 25, 24

} ;

#define MAX_OSC 4                      // max # of oscillators in DroneOscSection 

class DroneOscSection : public QuantumOsc 
{
   typedef QuantumOsc super;           // superclass is QuantumOsc

   protected:

   const word *periods;                // ptr to table of note periods in ROM
   byte  numNotes;                     // # of notes in periods[]

   byte  numOsc;                       // number of square-wave osc in section
   byte  curOsc;                       // current osc #

   byte  ampOsc;                       // amplitude of oscillators

   byte edgIdx[MAX_OSC];               // index into current edge 
   byte edgeDC[MAX_OSC];               // downcounter to next edge transition

   byte edgLen[MAX_OSC][2];            // edge lengths
   char edgVal[MAX_OSC][2];            // edge values

   byte note[MAX_OSC];                 // current note 

   byte pW;                            // pulse width: 1 to 128 (128=50%)

   public:

   const byte NoNote = 255;            // note # for no note

   bool  freeze;                       // if true freeze current notes

   DroneOscSection()
   {
      periods = &NotePeriods[0];
      numNotes = 48;
   }

   boolean charEv( char code )    
   {
      switch ( code )
      {
         #ifdef INTERN_CONSOLE

         case '0':                     // select current osc
         case '1':
         case '2':
         case '3':

            setCurOsc(code - '0');
            console.newlntab();
            console.infoByte( CONSTR("curOsc"), curOsc );
            console.newprompt();
            break;

         case 'F':                     // freeze-unfreeze notes

            console.getBool( CONSTR("Freeze"), &freeze );
            break;

         case 'n':                     // set note  
         {
            byte inp;
            if ( console.getByte( CONSTR("note"), &inp ) )
               setNote( curOsc, inp );
            break;
         }

         case 'p':                     // set phase  
         {
            int inp;
            if ( console.getInt( CONSTR("phase"), &inp ) && inp >= 0 )
               setPhase( curOsc, inp );
            break;
         }

         case 'N':                     // set # of osc  
         {
            byte inp;
            if ( console.getByte( CONSTR("NumOsc"), &inp ) )
               setNumOsc( inp );
            break;
         }

         case 'P':                     // set pulse width
         {
            byte inp;
            if ( console.getByte( CONSTR("PW"), &inp ) )
               setPW( inp );
            break;
         }
  
         #endif

         #ifdef CONSOLE_OUTPUT

         case chrInfo:                 // display object info to console

            super::charEv( chrInfo );
            console.newlntab();
            console.infoByte( CONSTR("curOsc"), curOsc );
            console.infoByte( CONSTR("NumOsc"), numOsc );
            console.infoBool( CONSTR("Freeze"), freeze );
            console.infoByte( CONSTR("PW"), pW );

            console.newlntab();
            for ( byte i = 0; i < numOsc; i++ )
               console.infoByte( noteStr(i), note[i] );

            console.newlntab();
            for ( byte i = 0; i < numOsc; i++ )
               console.infoInt( phaseStr(i), getPhase(i) );

            break;

         #endif

         case '!':                     // perform a reset

            super::charEv( code );  

            tracking = false;
            freeze   = false;
            pW       = 128;

            setNumOsc(1);

            for ( byte i = 0; i < MAX_OSC; i++ )
               setNote( i, NoNote );

            break;

         default:

            return super::charEv( code );  

      }
   }

   void output( char *buf )
   {
      byte icnt = audioBufSz;          // write this many ticks of output

      while ( icnt-- )                 // for each byte in the buffer
      {
         char outVal = 0;
         for ( byte t = 0; t < numOsc; t++ )
         {
            if ( --edgeDC[t] == 0 )       // handle edge transition, if any
            {
               edgIdx[t] ^= 1;            // toggle idx between 1 and 0
               edgeDC[t] = edgLen[t][edgIdx[t]];
            }
            outVal += edgVal[t][edgIdx[t]];
         }
         *buf++ = outVal;
      }
   }

   double realFreq()                   // return the real frequency
   {
      return periodHz( edgLen[0][0]+edgLen[0][1] );
   }

   byte getNumOsc()
   {
      return numOsc;
   }

   byte getCurOsc()
   {
      return curOsc;
   }

   void setCurOsc( byte ith )
   {
      if ( ith >= numOsc )
         ith = numOsc-1;
      curOsc = ith;
   }

   void setNote( byte t, byte noteNum )
   {
      if ( freeze )
         return;

      if ( noteNum == NoNote )
      {
         edgIdx[t] = 0;
         setSteps(t, 0, 0);
         note[t] = NoNote;
         return;
      }

      while ( noteNum >= numNotes )          // insure note falls within table
         noteNum -= 12;
      
      note[t] = noteNum;

      byte idx    = noteNum << 1;            // calc address of period in ROM
      word elePtr = (word )periods + idx;    // ""
      
      calcEdges( t, pgm_read_word_near( elePtr ) );

      /* phase lock with any identical osc */

      for ( byte i = 0; i < numOsc; i++ )
      {
         if ( t == i ) continue;
         if ( note[t] != note[i] ) continue;
         
         edgIdx[t] = edgIdx[i];
         edgeDC[t] = edgeDC[i];
      }

   }

   void setNumOsc( byte n )
   {
      if ( n == 0 ) 
         n = 1;

      if ( n > MAX_OSC )
         n = MAX_OSC;

      numOsc = n;

      if ( curOsc >= numOsc )
         curOsc = numOsc-1;

      ampOsc  = 127 / numOsc;

      for ( byte i = 0; i < numOsc; i++ )
      {
         edgVal[i][0] = ampOsc;
         edgVal[i][1] = (edgLen[i][0] && edgLen[i][1] ) ? -ampOsc : ampOsc;
      }
   }

   void calcEdges( byte t, word period )
   {
      Word posSteps; 
      Word negSteps;

      if ( period <= 1 )
      {
         setSteps( t, 0, 0 );
      }

      posSteps.val  = period;
      posSteps.val *= pW;
      posSteps.val  = posSteps._.msb;

      if ( posSteps.val == 0 ) 
         posSteps.val = 1;

      negSteps.val = period - posSteps.val;

      if ( negSteps.val > 255 )
      {
         posSteps.val += ( negSteps.val - 255 );
         negSteps.val = 255;
      }

      setSteps( t, posSteps._.lsb, negSteps._.lsb );
   }

   word getPhase( byte t )
   {
      word phase;
      phase  = edgIdx[t] ? 0 : edgLen[t][0];
      phase += edgLen[t][edgIdx[t]] - edgeDC[t];
      return phase;
   }

   void setPhase( byte t, word phase )
   {
      word period = edgLen[t][0] + edgLen[t][1];
      while ( phase >= period )
         phase -= period;

      if ( phase < edgLen[t][0] )
      {
         edgIdx[t] = 0;
         edgeDC[t] = edgLen[t][0]-phase;
      }
      else
      {
         edgIdx[t] = 1;
         edgeDC[t] = period-phase;
      }
   }

   void setPW( byte x )
   {
      if ( x < 1 ) 
          x = 1;
      if ( x > 128)
         x = 128;
      pW = x;

      for ( byte i = 0; i < numOsc; i++ )
         calcEdges( i, edgLen[i][0] + edgLen[i][1] );
   }

   void setSteps( byte t, byte n0, byte n1 )
   {
      edgLen[t][0] = n0;
      edgLen[t][1] = n1;
      edgVal[t][1] = ( n0 && n1 ) ? -ampOsc : ampOsc;
   }

   #ifdef CONSOLE_OUTPUT

   const char *noteStr( byte ith )                    
   {
      static const char ids[] PROGMEM = {"n0\0n1\0n2\0n3\0"};
      return &ids[ith*3];
   }

   const char *phaseStr( byte ith )                    
   {
      static const char ids[] PROGMEM = {"p0\0p1\0p2\0p3\0"};
      return &ids[ith*3];
   }

   #endif


   PROMPT_STR( oscSect ) 
} ;

/******************************************************************************
 *
 *                                  DroneVox 
 *
 ******************************************************************************/

class DroneVox : public Voice 
{
   typedef Voice super;                   // superclass is Voice

   public:

   DroneOscSection *oscSect;
   AutoWah         *autoWah;
   Gain            *gain;

   DroneVox()
   {
      oscSect = new DroneOscSection();
      useOsc( oscSect );
      autoWah = new AutoWah();
      addEffect( autoWah );
      gain = new Gain();
      addEffect( gain );
   }

   void noteOn( key note )                // turn a note on
   {
      note.transpose( xpose ); 
      byte noteNum = note.octave() * 12 + note.position();
      oscSect->setNote( oscSect->getCurOsc(), noteNum );
      trigger();
   }

} ;

// Note: the ambient and caustic oscillators are statically allocated here instead 
//       of dynamically allocated off the heap within LeadVox() to avoid an emergent
//       issue in Arduino 1.8.10+ where noise occurs if Beatitude synth is uploaded 
//       first, then Dronetic. In the absence of a (real) debugger it is unclear
//       why this should be.

FastWaveOsc staticAmbient;                // default "nice" oscillator
FastWaveOsc staticCaustic;                // "not-nice" oscillator

/******************************************************************************
 *
 *                                  LeadVox 
 *
 ******************************************************************************/

class LeadVox : public ADSRVoice 
{
   typedef ADSRVoice super;               // superclass is ADSRVoice

   public:

   FastWaveOsc *ambient;                  // default "nice" oscillator
   XorOsc      *caustic;                  // "nasty" osc used on some presets

   LeadVox()
   {
      // allocate ambient oscillator

      ambient = &staticAmbient;
      ambient->setTable( wavetable_named( Ether ) );
      useAmbient();

      // allocate caustic oscillator

      FastWaveOsc *waveOsc = &staticCaustic;
      waveOsc->setTable( wavetable_named( Ether ) );
      caustic = new XorOsc( waveOsc, new PureSquare() );
   }
   
   boolean charEv( char code )    
   {
      switch ( code )
      {
         #ifdef INTERN_CONSOLE

         case 'a':                     // use ambient oscillator

            useAmbient();
            break;

         case 'c':                     // use caustic oscillator

            useCaustic();
            break;

         #endif

         case '!':                     // perform a reset

            super::charEv( code );  

            ambient->reset();          // insure all oscillators are reset
            caustic->reset();

            useOsc( ambient );         // amient osc is default
            break;

         default:

            return super::charEv( code );  

      }
   }

   void useAmbient() 
   { 
      useOsc( ambient );
   }

   void useCaustic() 
   { 
      useOsc( caustic );
   }

} ;

/******************************************************************************
 *
 *                                 DroneSynth
 *
 ******************************************************************************/

class DroneSynth : public TwoVoxPanSynth
{
   typedef TwoVoxPanSynth super;          // superclass is TwoVoxPanSynth

   public:

   typedef byte playState ; enum
   { 

      STOPPED,       // playing is stopped
      FADE_IN,       // drone is fading in
      PLAYING,       // playing (at full volume)
      FADE_OUT       // drone is fading out

   } playStatus;                    // current play status

   ByteMenu presetMenu;             // keybrd menu for selecting presets

   DroneVox *drone[2];              // ptr to drone voices (same as vox[0..1])
   LeadVox  *lead;                  // ptr to lead voice (same as vox[2])

   Control *sendControl;            // send pot events to this control

   bool autoplay;                   // if false, ignore 'S' commands in presets

   void config()
   {
      configVoices(3);
      panControl = new PanControl( &this->panPos );
      setFrameDimensions( 2, 2 );
   }

   Tuning *tuning()
   {
      // use a harmonic tuning based on a F tonic (C on keybrd will sound as F)

      HarmonicTuning *harmonic = new HarmonicTuning();
      harmonic->tonic0 = 43.578;
      return harmonic;
   }

   // Note: newOsc() will never be called (back) by the system because all 
   // oscillators in this sketch are allocated and registered via useOsc() 
   // within their respective voice constructors, and the system only calls 
   // newOsc() for voices that have a NULL osc ptr after newVox() for that
   // voice has been called. However, we override newOsc() here simply to 
   // avoid having the newOsc() method of the super class linked in -- 
   // which costs an extra 400 bytes of ROM!

   Osc *newOsc( byte nth )                // create nth osc -- NEVER CALLED
   {
      return new Osc();                   
   }
      
   Voice *newVox( byte nth )              // create nth voice
   {
      switch ( nth )
      {
         case 0:   
         case 1:
                  drone[nth] = new DroneVox();
                  return drone[nth];
         default:  
                  lead = new LeadVox();
                  return lead;
      }
   }
      
   bool charEv( char code )                  // process a character event
   {
      switch ( code )
      {
         #ifdef INTERN_CONSOLE               // compile cases needed by macros

         case 'S':

            if ( autoplay ) start();
            break;

         case 'T':

            stop();
            break;

         #endif

         case '!':                           // perform a reset

            super::charEv( code );

            playStatus = STOPPED;
            setVol(0);
            flags &= ~RSTMUTE;

            enableFrame();                   // use embedded user-interface frame

            keybrd.setOctave(2);
            keybrd.setTopOct(3);

            execute( PSTR( "P<d32\\f.125\\" ) );
            execute( PSTR( "2ea60\\r200\\" ) );
            break;

         default:

            return super::charEv( code );  
      }
      return true;
   }

   void dynamics()                           // perform a dynamic update
   {
      if ( playStatus == FADE_IN )
      {
         if ( vol == 255 )
            playStatus = PLAYING;
         else
            setVol( vol+1 );
      }
      else if ( playStatus == FADE_OUT )
      {
         if ( vol == 0 )
            playStatus = STOPPED;
         else
            setVol( vol-1 );
      }
      super::dynamics();
   }

   bool evHandler( obEvent e )               // event handler
   {

      if ( handlePots(e) ) return true;      // handle any pot events

      switch ( e.type() )
      {     
         case BUT0_DTAP:                     // toggle drone on/off

            if ( playStatus == STOPPED || playStatus == FADE_OUT )
               start();
            else
               stop();
            break;

         case BUT1_DTAP:                     // run a preset 

            stop();                          // begin fading out drone

            // wait for user to select a new preset by pressing a key

            presetMenu.waitKey();

            // insure drone has faded completely out

            while ( playStatus != STOPPED )
               console.idle();

            // reduce CPU load to avoid audio glitch while loading new preset

            lead->useAmbient();              // uses less CPU then Caustic

            // run new preset

            runPreset( (const char *)presets.dataPtr( presetMenu.value ) );

            break;

         default:       

            return super::evHandler(e);      // pass other events through
      }
      return true;
   }

   bool handlePots( obEvent ev )             // handle pot events
   {
      if ( ! ev.amPot() )                    // return false if not a pot event
         return false;                       

      byte potVal = ev.getPotVal();          // cache pot value
      byte potNum = getPotNum( ev );         // cache pot # (sans frame #)

      switch ( ev.type() )                   // execute case by pot#_frame#
      {
         /*             FRAME 00             */

         case POT0_F00:                      // autowah 0 LFO freq 
         case POT1_F00:                      // autowah 0 LFO depth 

            drone[0]->autoWah->lfo.potEv(ev);
            break;

         /*             FRAME 10             */

         case POT0_F10:                      // autowah 1 LFO freq 
         case POT1_F10:                      // autowah 1 LFO depth 

            drone[1]->autoWah->lfo.potEv(ev);
            break;

         /*             FRAME 20             */

         case POT0_F20:                      // panning LFO freq 
         case POT1_F20:                      // panning LFO depth 

            panControl->potEv(ev);
            break;

         /*             FRAME 01             */

         case POT0_F01:                      // autowah 0 cutoff freq 
         case POT1_F01:                      // autowah 1 cutoff freq  

            drone[potNum]->autoWah->setCutoff( potVal );
            break;

         /*             FRAME 11             */

         case POT0_F11:                      // drone 0 pulse width
         case POT1_F11:                      // drone 1 pulse width 

            potVal >>= 1;                    // pulse widths range from 0 to 128 (50%)

            drone[potNum]->oscSect->setPW( potVal );
            break;

         /*             FRAME 21             */

         case POT0_F21:                      // transpose current osc of drone 0 
         case POT1_F21:                      // transpose current osc of drone 1 
         {
            double calcXpose = -12.0 + (double )potVal * (24.0/255.0);
            drone[potNum]->xpose = calcXpose;
            break;
         }

         /*             FRAME 02             */

         case POT0_F02:                      // set lead ADSR attack

            lead->envAmp.setAttack( potVal );
            break;
                                             // set lead ADSR decay
         case POT1_F02:                       

            lead->envAmp.setDecay( potVal );
            break;

         /*             FRAME 12             */

          case POT0_F12:                     // set lead ADSR sustain

            lead->envAmp.setSustain( potVal );
            break;
                                             
          case POT1_F12:                     // set lead ADSR release

            lead->envAmp.setRelease( potVal );
            break;
                                             // set lead ADSR decay

         /*             FRAME 22             */

         case POT0_F22:                      // set freqDiff  

            lead->caustic->freqDiff = potVal;
            break; 

         case POT1_F22:                      // set portamento  

            lead->setGlide( potVal );
            break; 

      }
      return true;
   }

   void noteOn( key note )                   // turn a note on
   {
      super::noteOn( note );
      if ( playStatus == STOPPED )
         start();
   }

   void output( char *bufL, char *bufR )
   {
      char  leadbuf[ audioBufSz ];          // temp buffer for lead vox output

      super::output( bufL, bufR );
      vox[2]->output( leadbuf );

      // mix 1/4 lead voice with each channel

      int sum;
      for ( byte i = 0 ; i < audioBufSz; i++ )
      {
         sum  = bufL[i];
         sum *= 3;
         sum += leadbuf[i];
         bufL[i] = sum >> 2;

         sum  = bufR[i];
         sum *= 3;
         sum += leadbuf[i];
         bufR[i] = sum >> 2;
      }

   }

   void start()
   {
      if ( playStatus != PLAYING )
         playStatus = FADE_IN;
   }

   void stop()
   {
      if ( playStatus != STOPPED )
         playStatus = FADE_OUT;
   }

   void welcome()
   {
      presets.load( myPresets );               

      // set up preset 0 (without playing it)

      autoplay = false;             
      runPreset( (const char *)presets.dataPtr( 0 ) ); 
      autoplay = true;
   }

} mySynth;

void setup()
{
   ardutouch_setup( &mySynth );                   
}

void loop()
{
   ardutouch_loop();                            
}                                             

                                             
