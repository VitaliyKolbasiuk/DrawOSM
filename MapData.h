#pragma once

// Allow any format of input files (XML, PBF, ...)
#include <osmium/io/any_input.hpp>

// We want to use the handler interface
#include <osmium/handler.hpp>

// Utility class gives us access to memory usage information
#include <osmium/util/memory.hpp>

// For osmium::apply()
#include <osmium/visitor.hpp>

#include <QPainter>
#include <vector>
#include <QPainterPath>
#include <QDebug>

#include <osmium/handler.hpp>
#include <osmium/index/index.hpp>
#include <osmium/index/map/dummy.hpp>
#include <osmium/index/node_locations_map.hpp>
#include <osmium/osm/location.hpp>
#include <osmium/osm/node.hpp>
#include <osmium/osm/node_ref.hpp>
#include <osmium/osm/types.hpp>
#include <osmium/osm/way.hpp>


class MapData : public osmium::handler::Handler
{
    //osmium::memory::Buffer* node_locations = new osmium::memory::Buffer(32);
public:
    std::vector<QPainterPath> m_pathVector;

    MapData()
    {
        m_pathVector.emplace_back();
        m_pathVector.back().moveTo(50, 150);  // Start point
        m_pathVector.back().lineTo(100, 50);

        m_pathVector.emplace_back();
        m_pathVector.back().moveTo(150, 200);  // Start point
        m_pathVector.back().lineTo(300, 250);
    }

    void node(const osmium::Node& node) noexcept
    {
    }

    void way(osmium::Way& way) noexcept
    {
        auto& nodes = way.nodes();

        for (const auto& nodeRef : nodes)
        {
            try {
                // Get the actual node using the node ID from the nodeRef
                //auto node = node_locations->get<osmium::NodeRef>(nodeRef.ref());

                // Check if the node has a location
                if (nodeRef.location())
                {
                    qDebug() << "x:" << nodeRef.x();
                    qDebug() << "y:" << nodeRef.y();
                    qDebug() << "lon:" << nodeRef.lon();
                    qDebug() << "lat:" << nodeRef.lat();
                }
                else {
                    //qDebug() << "Node has invalid or missing location information";
                }
            } catch (const std::exception& e) {
                qDebug() << "Exception while accessing coordinates:" << e.what();
            }
        }
    }

    void relation(const osmium::Relation& relation) noexcept {
    }
};


