import os

class FileManager():

    def __init__(self, torrent, destination):
        self.torrent = torrent
        self.destination = destination
        info_dict = self.get_files_info()
        self.create_dir_file(info_dict)
        self.complete = False

    def is_complete(self):
        return self.complete
    
    def get_files_info(self):
        
        if b'length' in self.torrent.info:

            length = self.torrent.info[b'length']
            name = self.torrent.info[b'name']
            mode = 'single'
            return {
                'length': length,
                'name': name,
                'mode': mode
            }

        else:
            multi_files = self.torrent.info[b'files']
            files = []

            for f in multi_files:
                files.append({
                    'name': f[b'path'][0],
                    'length': f[b'length'],
                    'length_written': 0,
                })

            files_info = {}
            files_info['dirname'] = self.torrent.info[b'name']
            files_info['files'] = files
            files_info['mode'] = 'multiple'
            return files_info

    def create_dir_file(self, info_dict):

        self.files = []
        if info_dict['mode'] == 'multiple':
            file_list = info_dict['files']
            dir_path = os.path.join(
                    os.path.expanduser(self.destination), info_dict['dirname'].decode('utf-8'))

            if not os.path.exists(dir_path):
                os.makedirs(dir_path)

            tmp_file_path = os.path.join(dir_path, 'tmp.tmp')
            if os.path.isfile(tmp_file_path):
                os.remove(tmp_file_path)
            self.tmp_file = open(tmp_file_path, 'wb')
            self.tmp_file_length = 0
            self.tmp_file_path = tmp_file_path

            for f in file_list:
                file_path = os.path.join(dir_path, f['name'].decode('utf-8'))

                if os.path.isfile(file_path):
                    os.remove(file_path)

                fd = open(file_path, 'wb')
                # fd.close()
                self.files.append({
                    'descriptor': fd,
                    'length_to_write': f['length'],
                })
        else:
            file_path = os.path.join(
                    os.path.expanduser(self.destination), info_dict['name'].decode('utf-8'))

            if os.path.isfile(file_path):
                os.remove(file_path)

            fd = open(file_path, 'wb')
            # fd.close()
            self.files.append({
                'descriptor': fd,
                'length_to_write': info_dict['length'],
            })

            tmp_file_path = file_path + '.tmp'
            if os.path.isfile(tmp_file_path):
                os.remove(tmp_file_path)
            self.tmp_file = open(tmp_file_path, 'wb')
            self.tmp_file_length = 0
            self.tmp_file_path = tmp_file_path

    def write(self, piece_index, data):
        """
        because pieces dont come in in order,
        we need to put pieces in order in a temporary file first.
        once we have all the pieces in order in the temporary file,
        write to actual files in order
        """
        
        offset = piece_index * self.torrent.piece_length(0)
        self.tmp_file.seek(offset)
        self.tmp_file.write(data)
        self.tmp_file_length += len(data)
        total_file_length = self.torrent.file_length()
        print('writing to tmp file: {}, total length: {}'.format(os.path.getsize(self.tmp_file_path), total_file_length))
        if self.tmp_file_length == total_file_length:
            # close tmp file for writing
            self.tmp_file.close()
            # open for reading
            with open(self.tmp_file_path, 'r+b') as fd:
                content = fd.read()
                print(len(content))
                self.write_to_file(content)
                self.complete = True
                os.remove(self.tmp_file_path)

    def write_to_file(self, content):

        print('start writing to actual files')
        for f in self.files:
            fd = f['descriptor']
            length_to_write = f['length_to_write']
            fd.seek(0)
            fd.write(content[:length_to_write])
            content = content[length_to_write:]

            print('at pos {}'.format(fd.tell()))
            print('length to write {}'.format(length_to_write))
            print('wrote length: {}'.format(len(content[:length_to_write])))
            print('finished writing to {}'.format(fd.name))
            print('remaining content length: {}'.format(len(content[length_to_write:])))

            fd.close()
