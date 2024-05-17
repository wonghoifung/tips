import requests
import hashlib
import bencodepy

def get_torrent_from_tracker(info_hash,tracker_url):
    params = {
        'info_hash': info_hash
    }
    try:
        response = requests.get(tracker_url, params=params, timeout=5)
        if response.status_code == 200:
            print("success response:{}".format(response.content))
            return response.content 
        print("failed response:{}".format(response))
    except requests.exceptions.Timeout:
        print("tracker timeout:{}".format(tracker_url))
    except requests.exceptions.RequestException as e:
        print("tracker error:{}, {}".format(e, tracker_url))
    return None

tracker_list=[]

def calculate_info_hash_bin(torrent_path):
    with open(torrent_path, 'rb') as f:
        torrent_data = f.read()
        metadata = bencodepy.decode(torrent_data)
        announce_list = metadata.get(b'announce-list', None)
        if announce_list:
            for announce in announce_list:
                tracker_list.append(announce[0].decode('utf-8'))
        info = metadata[b'info']
        info_hash = hashlib.sha1(bencodepy.encode(info)).digest()
        return info_hash
    
if __name__ == '__main__':
    torrent_path = "/home/hhf/Desktop/test/bt/6f36c48a1b418e96b736359045a9b2cabba75281.torrent"
    info_hash = calculate_info_hash_bin(torrent_path)
    # tracker_list=['http://127.0.0.1:6969/announce']

    while True:
        if len(tracker_list) == 0:
            print('no tracker left')
            break
        tracker_url=tracker_list.pop()
        if not tracker_url.startswith('http'):
            continue
        torrent_file = get_torrent_from_tracker(info_hash, tracker_url)
        if torrent_file:
            with open('downloaded.torrent', 'wb') as f:
                f.write(torrent_file)
            print('Torrent file downloaded successfully with tracker:{}'.format(tracker_url))
            t = bencodepy.decode(torrent_file)
            print('t:{}'.format(t))
            peers = t.get(b'peers', None)
            if peers:
                peer_list = []
                if type(peers) == type('') or type(peers) == type(b''):
                    peers=str(peers)
                    for i in range(0,len(peers)-5,6):
                        one_peer = peers[i:i+6]
                        ip = one_peer[0:4]
                        port = one_peer[4:6]
                        ip = '%d.%d.%d.%d' %(ord(ip[0]),ord(ip[1]),ord(ip[2]),ord(ip[3]),)
                        port = ord(port[0])*256+ord(port[1])
                        peer_list.append((ip,port))
                elif type(peers) == type([]):
                    for peer in peers:
                        peer_list.append((peer['ip'],peer['port']))
                print(peer_list)
            break
        else:
            print('Failed to download Torrent file')

# response      
# (
#     [
#         (b'complete', 0), 
#         (b'downloaded', 0), 
#         (b'incomplete', 2), 
#         (b'interval', 1950), 
#         (b'min interval', 975), 
#         (b'peers', b'\xac\x16\x00\x01\x00\x00\xac\x16\x00\x01Ms')
#     ]
# )
