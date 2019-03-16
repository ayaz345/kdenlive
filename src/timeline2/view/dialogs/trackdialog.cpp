/***************************************************************************
 *   Copyright (C) 2017 by Jean-Baptiste Mardelle (jb@kdenlive.org)        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

#include "trackdialog.h"

#include "kdenlivesettings.h"

#include <QIcon>

TrackDialog::TrackDialog(const std::shared_ptr<TimelineItemModel> &model, int trackIndex, QWidget *parent, bool deleteMode)
    : QDialog(parent)
    , m_audioCount(1)
    , m_videoCount(1)
{
    setWindowTitle(deleteMode ? i18n("Delete Track") : i18n("Add Track"));
    // setFont(QFontDatabase::systemFont(QFontDatabase::SmallestReadableFont));
    QIcon videoIcon = QIcon::fromTheme(QStringLiteral("kdenlive-show-video"));
    QIcon audioIcon = QIcon::fromTheme(QStringLiteral("kdenlive-show-audio"));
    setupUi(this);
    QStringList existingTrackNames;
    for (int i = model->getTracksCount() - 1; i >= 0; i--) {
        int tid = model->getTrackIndexFromPosition(i);
        bool audioTrack = model->isAudioTrack(tid);
        if (audioTrack) {
            m_audioCount++;
        } else {
            m_videoCount++;
        }
        const QString trackName = model->getTrackFullName(tid);
        existingTrackNames << trackName;
        comboTracks->addItem(audioTrack ? audioIcon : videoIcon, trackName.isEmpty() ? QString::number(i) : trackName, tid);
        // Track index in in MLT, so add + 1 to compensate black track
        m_positionByIndex.insert(tid, i + 1);
    }
    if (trackIndex > -1) {
        int ix = comboTracks->findData(trackIndex);
        comboTracks->setCurrentIndex(ix);
        if (model->isAudioTrack(trackIndex)) {
            audio_track->setChecked(true);
            before_select->setCurrentIndex(1);
        }
    }
    trackIndex--;
    if (deleteMode) {
        track_name->setVisible(false);
        video_track->setVisible(false);
        audio_track->setVisible(false);
        name_label->setVisible(false);
        before_select->setVisible(false);
        label->setText(i18n("Delete Track"));
    } else {
        // No default name since we now use tags
        /*QString proposedName = i18n("Video %1", trackIndex);
        while (existingTrackNames.contains(proposedName)) {
            proposedName = i18n("Video %1", ++trackIndex);
        }
        track_name->setText(proposedName);*/
    }
}

int TrackDialog::selectedTrackPosition() const
{
    if (comboTracks->count() > 0) {
        int position = m_positionByIndex.value(comboTracks->currentData().toInt());
        if (before_select->currentIndex() == 1) {
            position--;
        }
        return position;
    }
    return -1;
}

int TrackDialog::selectedTrackId() const
{
    if (comboTracks->count() > 0) {
        return comboTracks->currentData().toInt();
    }
    return -1;
}

bool TrackDialog::addAudioTrack() const
{
    return !video_track->isChecked();
}
const QString TrackDialog::trackName() const
{
    return track_name->text();
}