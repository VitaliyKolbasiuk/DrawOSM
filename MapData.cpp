#include "MapData.h"

void MapData::read()
{
    const osmium::io::File input_file{"/home/vitaliykolbasiuk/Downloads/odessa.osm.pbf"};
    osmium::io::Reader reader{input_file, osmium::osm_entity_bits::type::all};

    // Create an instance of our own CountHandler and push the data from the
    // input file through it.
    osmium::apply(reader, *this);

    // You do not have to close the Reader explicitly, but because the
    // destructor can't throw, you will not see any errors otherwise.
    reader.close();
}
