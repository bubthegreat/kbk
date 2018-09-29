#!/usr/bin/perl -w

use strict;
use IO::Socket;

my $port = 4000;
my $exec = "./pos2";
my $check_interval = 30;
my $this_script = "perl auto_start.pl";

print "Auto-start initialized, check interval on port $port set to $check_interval.\n\r";
for (;;) {
	#Attempt to create a socket to the kbk port and check the result.
	my $sock = new IO::Socket::INET(LocalAddr 	=> 'localhost', 
									LocalPort 	=> $port,
									Proto 		=> 'tcp');

	if ($sock) {
		close $sock or die "Unable to close socket. Can't start it now!\n\r";
		print "Auto-start: KBK appears to be down. Attempting to auto-restart.\n\r";
		system($exec);
		system($this_script);
	}
	else {
		print "Auto-start: KBK appears to be up.\n\r";
	}
	sleep(30);
}
