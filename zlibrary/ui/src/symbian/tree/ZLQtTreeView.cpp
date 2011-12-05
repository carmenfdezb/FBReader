#include <QtCore/QDebug>
#include <QtGui/QCommonStyle>

#include "ZLQtTreeModel.h"
#include "ZLQtTreeView.h"
#include "../dialogs/ZLQtProgressDialog.h"

ZLQtTreeView::ZLQtTreeView(QWidget *parent) :  QListView(parent) {

//    myLoadingIcon = new LoadingIconV2(this);

//    connect(myLoadingIcon, SIGNAL(rotated()), this, SLOT(paint()));
//    //connect(myLoadingIcon, SIGNAL(rotated()), this, SLOT(update());

//    this->setUpdatesEnabled(true);

}

void ZLQtTreeView::paintEvent(QPaintEvent *event) {
    //qDebug() << Q_FUNC_INFO;
    QListView::paintEvent(event);
//    QPixmap currentFrame = myLoadingIcon->getPixmap();

//    QRect frameRect = currentFrame.rect();

//   // Only redraw when the frame is in the invalidated area
//   frameRect.moveCenter(rect().center());
//   if (frameRect.intersects(event->rect()))
//   {
//      QPainter painter(this->viewport());
//      painter.drawPixmap(
//         frameRect.left(),
//         frameRect.top(),
//         currentFrame);
//   }

//    QPainter painter2(this->viewport());
//    painter2.drawPixmap(0,0,myPixmap);
//    painter2.end();
}

void ZLQtTreeView::paint() {
    qDebug() << Q_FUNC_INFO;
    update();
}

SubtitleDelegate::SubtitleDelegate(QObject *parent) : QStyledItemDelegate(parent) {

}

void SubtitleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    //qDebug() << Q_FUNC_INFO << index.data() << index.data(ZLQtTreeModel::SubTitleRole);
    //QStyledItemDelegate::paint(painter,option,index);
    //qDebug() << Q_FUNC_INFO << option.rect;

    drawHighlight(painter,option,index);
    drawIcon(painter,option,index);
    drawTitle(painter,option,index);
    drawSubtitle(painter,option,index);

    //TODO clear it up before compile for Symbian
    //draw border
#ifndef	__SYMBIAN__
    painter->save();
    painter->setPen(Qt::black);
    painter->drawPolygon(QPolygon(option.rect, true),Qt::WindingFill);
    painter->restore();
#endif


}

void SubtitleDelegate::drawTitle(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QString title = index.data(Qt::DisplayRole).toString();
    QString subtitle = index.data(ZLQtTreeModel::SubTitleRole).toString();
    if (title.isEmpty()) {
        return;
    }
    painter->save();
    painter->setClipRect(option.rect);
    painter->setPen( option.state & QStyle::State_HasFocus ?
                     option.palette.highlightedText().color() :
                     option.palette.text().color());

    QRect textRect;
    int alignFlags = Qt::AlignCenter;
    if (subtitle.isEmpty()) {
        textRect = option.rect;
        alignFlags = Qt::AlignVCenter;
    } else {
        int centerY = option.rect.bottom() - option.rect.size().height()/2;
        textRect = QRect(option.rect.topLeft(), QPoint(option.rect.right(), centerY));
        alignFlags = Qt::AlignBottom;
    }
    textRect.adjust(getTextLeftMargin(option, index), 0,0,0);

    painter->setFont(MenuItemParameters::getFont());
    QString elidedSubtitle = painter->fontMetrics().elidedText(title,Qt::ElideRight, textRect.width());
    painter->drawText(textRect, alignFlags,  elidedSubtitle);
    painter->restore();
}

void SubtitleDelegate::drawSubtitle(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QString subtitle = index.data(ZLQtTreeModel::SubTitleRole).toString();
    if (subtitle.isEmpty()) {
        return;
    }
    painter->save();
    painter->setClipRect(option.rect);
    painter->setPen( option.state & QStyle::State_HasFocus ?
                     option.palette.highlightedText().color() :
                     option.palette.text().color());
    //QRect textRect = option.rect;//.adjusted(textMargin+iconMargin, 0, -(textMargin+iconMargin), 0);
    int centerY = option.rect.top() + option.rect.size().height()/2;
    QRect textRect(QPoint(option.rect.left(), centerY), option.rect.bottomRight());

    textRect.adjust(getTextLeftMargin(option, index), 0,0,0);

    painter->setFont(MenuItemParameters::getSubtitleFont());
    QString elidedSubtitle = painter->fontMetrics().elidedText(subtitle,Qt::ElideRight, textRect.width());
    painter->drawText(textRect, Qt::AlignVCenter,  elidedSubtitle);
    painter->restore();
}

void SubtitleDelegate::drawIcon(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QPixmap pixmap = index.data(Qt::DecorationRole).value<QPixmap>();
    if (pixmap.isNull()) {
        return;
    }
    //painter->drawPixmap(QRect(option.rect.topLeft(), pixmap.size()) , pixmap);
    QPoint topLeft = QPoint( option.rect.left() + getLeftMargin(), option.rect.top());
    painter->drawPixmap( getCenteredRectangle( QRect(topLeft, MenuItemParameters::getImageZoneSize()),
                                               pixmap.size()) , pixmap);
    //painter->drawPixmap(QRect(iconRect.topLeft(), pixmap.size()), pixmap);

}

void SubtitleDelegate::drawHighlight(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    const QStyleOptionViewItemV4 *vopt = qstyleoption_cast<const QStyleOptionViewItemV4 *>(&option);
    QStyleOptionViewItemV4 voptAdj = *vopt;
    const QWidget* widget = voptAdj.widget;
    const QAbstractItemView *itemView = qobject_cast<const QAbstractItemView *>(widget);

    QStyle *style = widget ? widget->style() : QApplication::style();

    //painter->setClipRect(voptAdj.rect);
    const bool isSelected = (vopt->state & QStyle::State_Selected);
    const bool hasFocus = (vopt->state & QStyle::State_HasFocus);

    const bool singleSelection = itemView &&
        ((itemView->selectionMode() == QAbstractItemView::SingleSelection ||
         itemView->selectionMode() == QAbstractItemView::NoSelection));
    const bool selectItems = itemView && (itemView->selectionBehavior() == QAbstractItemView::SelectItems);

    //TODO this code has been brought from QS60Style and may be should be refactored
    // draw themed background for itemview unless background brush has been defined.
    if (vopt->backgroundBrush == Qt::NoBrush) {
        if (itemView) {
            //With single item selection, use highlight focus as selection indicator.
            if (singleSelection && isSelected){
                voptAdj.state = voptAdj.state | QStyle::State_HasFocus;
                if (!hasFocus && selectItems) {
                    painter->save();
                    painter->setOpacity(0.5);
                }
            }
            style->drawPrimitive(QStyle::PE_PanelItemViewItem, &voptAdj, painter, widget);
            if (singleSelection && isSelected && !hasFocus && selectItems) {
                painter->restore();
            }
        }
        //in original QS60Style, there was a line
        //} else { QCommonStyle::drawPrimitive(QStyle::PE_PanelItemViewItem, &voptAdj, painter, widget);}
    } else { style->drawPrimitive(QStyle::PE_PanelItemViewItem, &voptAdj, painter, widget);}


}

QSize SubtitleDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    //qDebug() << Q_FUNC_INFO << index << QStyledItemDelegate::sizeHint(option,index) << MenuItemParameters::getItemSize();
    QPixmap pixmap = index.data(Qt::DecorationRole).value<QPixmap>();
    if (pixmap.isNull()) {
        qDebug() << Q_FUNC_INFO << "pixmap is null";
        return MenuItemParameters::getSmallItemSize();
    }
    return MenuItemParameters::getItemSize();
    //return QStyledItemDelegate::sizeHint(option,index);
}

QRect SubtitleDelegate::getCenteredRectangle(QRect zoneRect, QSize imageSize) {
    if (zoneRect.width() < zoneRect.width() || zoneRect.height() < zoneRect.height() ) {
        qDebug() << "SubtitleDelegate::getCenteredRectangle -- imageSize is more then zoneRect" << zoneRect << imageSize;
        return zoneRect;
    }
    return QRect( QPoint( zoneRect.left() + (zoneRect.width() - imageSize.width())  / 2,
                          zoneRect.top()  + (zoneRect.height()- imageSize.height()) / 2 ),
                          imageSize);
}

int SubtitleDelegate::getTextLeftMargin(const QStyleOptionViewItem &option, const QModelIndex &index) {
//    qDebug() << "GET_TEXT_LEFT_MARGIN";
//    qDebug() << QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin, &option); // 3
//    qDebug() << QApplication::style()->pixelMetric(QStyle::PM_DefaultLayoutSpacing, &option); // 6
//    const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin, &option) + 2;
//    const int iconMargin = MenuItemParameters::getImageZoneSize().width() +
//                           QApplication::style()->pixelMetric(QStyle::PM_DefaultLayoutSpacing, &option);
    int textMargin = 0;
    int iconMargin = 0;

    QPixmap pixmap = index.data(Qt::DecorationRole).value<QPixmap>();
    if (!pixmap.isNull()) {
        iconMargin = MenuItemParameters::getImageZoneSize().width(); //+
                     //QApplication::style()->pixelMetric(QStyle::PM_DefaultLayoutSpacing, &option);
    }
    return getLeftMargin() + textMargin + iconMargin;
}

int SubtitleDelegate::getLeftMargin() {
    static const int LEFT_MARGIN = 6;
    return LEFT_MARGIN;
}
