#!/usr/bin/perl

use strict;
use warnings;

use IO::Socket::INET;

my $socket = IO::Socket::INET::->new(
	PeerAddr => '127.0.0.1',
	PeerPort => '8080',
	Proto    => 'tcp',
);

print $socket "POST /put/id3 HTTP/1.1\r\n"; 
# print $socket "User-Agent: curl/7.15.5 (i686-pc-linux-gnu) libcurl/7.15.5 OpenSSL/0.9.7j zlib/1.2.3\r\n";
print $socket "Host: 127.0.0.1\r\n"; 
print $socket "Accept: */*\r\n"; 
print $socket "Content-Length: " . (65536 * 2) . "\r\n"; 
print $socket "Content-Type: application/octet-stream\r\n"; 
print $socket "Connection: close\r\n";
print $socket "\r\n"; 

# sleep(1);
print $socket '*' x 65536;

# sleep(1);
print $socket '*' x 65536;

my @buffer = <$socket>;
close ($socket);

print @buffer;

