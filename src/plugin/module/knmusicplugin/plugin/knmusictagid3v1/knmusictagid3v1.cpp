/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QTextCodec>

#include "knglobal.h"

#include "knmusictagid3v1.h"

KNMusicTagID3v1::KNMusicTagID3v1(QObject *parent) :
    KNMusicTagParser(parent)
{
    m_defaultCodec=KNGlobal::localeDefaultCodec();
}

bool KNMusicTagID3v1::praseTag(QFile &musicFile,
                               QDataStream &musicDataStream,
                               KNMusicDetailInfo &detailInfo)
{
    //Check is the file size is enough.
    int fileSize=musicFile.size();
    //ID3v1 is 128 bytes, so if the file size is less than 128, it can't have
    //ID3v1.
    if(fileSize<128)
    {
        return false;
    }
    //Create the tag caches.
    char rawTagData[128];
    ID3v1Struct tagData;
    //Read the raw tag data.
    musicDataStream.skipRawData(fileSize-128);
    musicDataStream.readRawData(rawTagData, 128);
    //Check is the header 'TAG':
    if(rawTagData[0]!='T' || rawTagData[1]!='A' || rawTagData[2]!='G')
    {
        return false;
    }
    //Parse the raw data.
    parseRawData(rawTagData, tagData);
    //Write raw data to the detail info.
    writeTagDataToDetailInfo(tagData, detailInfo);
    return true;
}

bool KNMusicTagID3v1::parseAlbumArt(KNMusicDetailInfo &detailInfo)
{
    //ID3v1 doesn't contains album art.
    Q_UNUSED(detailInfo)
    return false;
}

void KNMusicTagID3v1::parseRawData(char *rawTagData,
                                   ID3v1Struct &tagData)
{
    char lastBackupData;
    int lastBackupPosition=-1;
    //ID3v1 is very simple, its data is stored in order:
    //  30 bytes Title + 30 bytes Artist + 30 bytes Album + 4 bytes Year +
    //  30 bytes Comments(+Track) + 1 bytes Genre
    //Just read them by order.
    backupByte(rawTagData, 33, lastBackupData, lastBackupPosition, true);
    //Title
    tagData.tags[0]=m_defaultCodec->toUnicode(rawTagData+3).simplified();
    backupByte(rawTagData, 63, lastBackupData, lastBackupPosition, true);
    //Artist
    tagData.tags[1]=m_defaultCodec->toUnicode(rawTagData+33, 30).simplified();
    backupByte(rawTagData, 93, lastBackupData, lastBackupPosition, true);
    //Album
    tagData.tags[2]=m_defaultCodec->toUnicode(rawTagData+63, 30).simplified();
    backupByte(rawTagData, 97, lastBackupData, lastBackupPosition, true);
    //Year
    tagData.tags[3]=m_defaultCodec->toUnicode(rawTagData+93, 4).simplified();
    backupByte(rawTagData, 127, lastBackupData, lastBackupPosition, true);
    //Comment is a little complex: check the No.125 char first, if it's 0, then
    //the following char is track index.
    if(rawTagData[125]==0)
    {
        tagData.track=(quint8)rawTagData[126];
    }
    tagData.tags[4]=m_defaultCodec->toUnicode(rawTagData+97).simplified();
    //Genre index.
    tagData.genreIndex=lastBackupData;
}

void KNMusicTagID3v1::writeTagDataToDetailInfo(const ID3v1Struct &tagData,
                                               KNMusicDetailInfo &detailInfo)
{
    //Set texts
    detailInfo.textLists[Name]=tagData.tags[0];
    detailInfo.textLists[Artist]=tagData.tags[1];
    detailInfo.textLists[Album]=tagData.tags[2];
    detailInfo.textLists[Year]=tagData.tags[3];
    detailInfo.textLists[Comments]=tagData.tags[4];
    detailInfo.textLists[Genre]=
            KNMusicGlobal::instance()->indexedGenre(tagData.genreIndex);
    //Set track
    if(tagData.track!=-1)
    {
        detailInfo.textLists[TrackNumber]=QString::number(tagData.track);
    }
}

void KNMusicTagID3v1::backupByte(char *rawTagData,
                                 const int &backupPosition,
                                 char &backupPool,
                                 int &positionPool,
                                 const bool &clearData)
{
    //Check the position backup pool, if it's not null(-1), restore the data.
    if(positionPool!=-1)
    {
        rawTagData[positionPool]=backupPool;
    }
    //Do the backup.
    backupPool=rawTagData[backupPosition];
    positionPool=backupPosition;
    //If ask to clear the data, clear it.
    if(clearData)
    {
        rawTagData[backupPosition]=0;
    }
}