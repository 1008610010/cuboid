#!/usr/bin/perl

use strict;
use warnings;

use ExtUtils::testlib;
use Data::Dumper;
use SUP::XTL;
use Fcntl;
use Digest::MD5 qw(md5 md5_hex md5_base64);

# my $i = SUP::XTL::int64();
# print Dumper($i), "\n";

my $is = SUP::XTL::FileInputStream->new("/data/stats/hit_stats/table/journal_stats_0-2012-01.dat");

print $is->size(), ' ', $is->position(), "\n";

my $buffer;
my $ctx = Digest::MD5->new();
while (!$is->at_end())
{
	my $to_read = min(int(rand(65536)) + 1, $is->size_left());
	# print "$to_read\n";
	$is->read($buffer, $to_read);
	# printf "%d\n", length($buffer);
	$ctx->add($buffer);
}

print $ctx->hexdigest(), "\n";



sub to_hex_string
{
	my $s = shift;
	my @bytes = unpack('C*', $s);
	my $result = '';
	foreach (@bytes)
	{
		$result .= int_to_hex($_ >> 4) . int_to_hex($_ & 0x0f) . ' ';
		# $result .= int_to_hex($_ >> 4) . int_to_hex($_ & 0x0f) . ' ';
	}
	return $result;
}

sub int_to_hex
{
	my $i = shift;

	return $i < 10 ? chr(ord('0') + $i) : chr(ord('A') + $i - 10);
}

sub min
{
	return $_[0] < $_[1] ? $_[0] : $_[1];
}


