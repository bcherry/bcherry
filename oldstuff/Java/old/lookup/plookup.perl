#!/usr/local/bin/perl -w
# $Id: plookup.perl,v 350.3 2005-01-20 11:53:34-08 - - $
#
# Perl solution to `lookup-list' assignment.
#

$0 =~ s|^(.*/)?([^/]+)/*$|$2|;
BEGIN {$EXITCODE = 0}
END {exit $EXITCODE}
sub note(@) {print STDERR "$0: @_"}
$SIG{'__WARN__'} = sub {note @_; $EXITCODE = 1};
$SIG{'__DIE__'} = sub {warn @_; exit};

sub trim($) {$_ = $_[0]; s/^\s*//; s/\s*$//; $_}

sub printkv($) {printf "%s = %s\n", @{$LIST[$_[0]]}}

sub printall() {printkv $_ for 0..$#LIST}

sub printval() {printkv $_ for grep {$LIST[$_][1] eq $val} 0..$#LIST}

sub printfind() {
   map {printkv $_ and return $_ if $LIST[$_][0] eq $key} 0..$#LIST;
   return undef;
}

sub keynull() {splice @LIST, $pos, 1 if defined ($pos = printfind)}

sub keyval () {
   if (defined ($pos = printfind)) {$LIST[$pos][1] = $val}
                              else {unshift @LIST, [$key, $val]}
}

@switch = (\&printall, \&printval, \&keynull, \&keyval);

$wantecho = 1, shift if @ARGV and $ARGV[0] eq "-e";
while ($line = <>) {
   print "$0: $line" if $wantecho;
   next if $line =~ m/^\s*(#|$)/;
   ($key, $val) = map {trim $_} split m/=/, $line, 2;
   if (defined $val) {&{$switch[ ($key ne "") * 2 + ($val ne "") ]}}
                else {printfind}
}

