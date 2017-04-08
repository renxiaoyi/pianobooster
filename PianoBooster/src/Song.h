/*********************************************************************************/
/*!
@file           Song.h

@brief          xxx.

@author         L. J. Barman

    Copyright (c)   2008-2009, L. J. Barman, all rights reserved

    This file is part of the PianoBooster application

    PianoBooster is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PianoBooster is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PianoBooster.  If not, see <http://www.gnu.org/licenses/>.

*/
/*********************************************************************************/

#ifndef __SONG_H__
#define __SONG_H__

#include <vector>

#include <QString>

#include "Notation.h"
#include "Conductor.h"
#include "TrackList.h"

#define PC_KEY_LOWEST_NOTE      21
#define PC_KEY_HIGHEST_NOTE    110

struct pcNote_s
{
    int key;
    int note;
};

static const pcNote_s pcNoteLookup[] =
{
    { 'z', 57 }, // A
    { 's', 58 },
    { 'x', 59 }, // B

    { 'c', 60 }, // middle C (c1)
    { 'f', 61 },
    { 'v', 62 }, // D
    { 'g', 63 },
    { 'b', 64 }, // E
    { 'n', 65 }, // F
    { 'j', 66 },
    { 'm', 67 }, // G
    { 'k', 68 },
    { ',', 69 }, // A
    { 'l', 70 },
    { '.', 71 }, // B

    { '/', 72 }, // C (c2)
    { '\'', 73 },

    { 'q', 67 }, // G
    { '2', 68 },
    { 'w', 69 }, // A
    { '3', 70 },
    { 'e', 71 }, // B

    { 'r', 72 }, // C (c2)
    { '5', 73 },
    { 't', 74 }, // D
    { '6', 75 },
    { 'y', 76 }, // E
    { 'u', 77 }, // F
    { '8', 78 },
    { 'i', 79 }, // G
    { '9', 80 },
    { 'o', 81 }, // A
    { '0', 82 },
    { 'p', 83 }, // B

    { '[', 84 }, // C (c3)
    { '=', 85 },
    { ']', 86 }, // D
};

class CSong : public CConductor
{
public:
    CSong()
    {
        CStavePos::setKeySignature( NOT_USED, 0 );
        m_midiFile = new CMidiFile;
        m_trackList = new CTrackList;

        reset();
    }

    ~CSong()
    {
        delete m_midiFile;
        delete m_trackList;
    }

    void reset()
    {
        m_reachedMidiEof = false;
        m_findChord.reset();
        m_pcNoteLookup.clear();
        for (size_t i = 0; i < arraySize(pcNoteLookup); i++)
          m_pcNoteLookup.push_back(pcNoteLookup[i]);
    }

    void init2(CScore* scoreWin, CSettings* settings);
    eventBits_t task(int ticks);
    bool pcKeyPress(int key, bool down);
    void loadSong(const QString &filename);
    void regenerateChordQueue();

    void rewind();

    void playFromStartBar()
    {
        rewind();
        playMusic(true);
    }

    void setActiveHand(whichPart_t hand);
    whichPart_t getActiveHand() { return CNote::getActiveHand(); }

    void setActiveChannel(int part);
    void setPlayMode(playMode_t mode);
    CTrackList* getTrackList() { return m_trackList; }
    QString getSongTitle() { return m_songTitle; }

    void refreshScroll();
    void refreshPCNoteRange(int delta)
    {
        for (size_t i = 0; i < m_pcNoteLookup.size(); i++)
          m_pcNoteLookup[i].note = pcNoteLookup[i].note + 12*delta;
    }

private:
    void midiFileInfo();

    CMidiFile* m_midiFile;
    CFindChord m_findChord;
    bool m_reachedMidiEof;
    CTrackList* m_trackList;
    QString m_songTitle;
    std::vector<pcNote_s> m_pcNoteLookup;
};

#endif  // __SONG_H__
