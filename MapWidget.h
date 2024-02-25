#pragma once

#include <QPainter>
#include <QWidget>
#include <QPainterPath>

#include "MapData.h"

class MapWidget : public QWidget
{
    MapData m_mapData;
public:
    MapWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        const osmium::io::File input_file{"/home/vitaliykolbasiuk/Downloads/odesa_oblast.osm.pbf"};
        osmium::io::Reader reader{input_file, osmium::osm_entity_bits::type::all};

        // Create an instance of our own CountHandler and push the data from the
        // input file through it.
        osmium::apply(reader, m_mapData);

        // You do not have to close the Reader explicitly, but because the
        // destructor can't throw, you will not see any errors otherwise.
        reader.close();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);

        QPainter painter(this);
        QPen pen(Qt::blue);
        pen.setWidth(2);
        painter.setPen(pen);

        for (const auto& path : m_mapData.m_pathVector)
        {
            painter.drawPath(path);
        }
    }
};
