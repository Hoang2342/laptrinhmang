from ftplib import FTP, error_perm
import socket

def upload_file(hostname, username, password, filename):
    ftp = FTP()
    ftp.connect(hostname)
    ftp.login(username, password)
    
    try:
        ftp.set_pasv(True)  # S? d?ng l?nh PASV
    except AttributeError:
        # H? tr? EPSV cho các phiên b?n Python cu hon
        ftp.sock = socket.create_connection((ftp.host, ftp.port), ftp.timeout, ftp.source_address)
        ftp.af = ftp.sock.family
        ftp.sock.sendall(b'EPSV\r\n')
        resp = ftp.getresp()
        if resp.startswith(b'229'):
            parts = resp.split(b' ')
            epsv_port = int(parts[-1][:-2])
            ftp.sock = socket.create_connection((ftp.host, epsv_port), ftp.timeout, ftp.source_address)
            ftp.af = ftp.sock.family
    
    with open(filename, 'rb') as file:
        try:
            ftp.storbinary('STOR ' + filename, file)
            print(f'File "{filename}" uploaded successfully.')
        except error_perm as e:
            print(f'Error: {str(e)}')
    
    ftp.quit()

# S? d?ng hàm upload_file d? t?i file lên server
hostname = 'ftp.example.com'
username = 'your_username'
password = 'your_password'
filename = 'file_to_upload.txt'

upload_file(hostname, username, password, filename)

