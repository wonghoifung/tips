import argparse
import asyncio

from bittorrent.client import TorrentClient

def main(torrent_file, destination):
    loop = asyncio.get_event_loop()
    client = TorrentClient(torrent_file, destination, loop)

    asyncio.ensure_future(client.connect_to_peers())
    asyncio.ensure_future(client.keep_alive())

    try:
        loop.run_forever()
    except KeyboardInterrupt:
        pass
    finally:
        loop.close()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='CLI BitTorrent Client')
    parser.add_argument('source', help='locaiton of the torrent file')
    parser.add_argument('destination', help='specify location of downloaded files')
    args = parser.parse_args()

    main(args.source, args.destination)
