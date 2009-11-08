#!/usr/local/bin/perl -w
# $Id: edfile.perl,v 351.1 2005-02-24 18:30:35-08 - - $
# See the listedit utility man page for specifications.

$0 =~ s{.*/}{};
select STDOUT; $| = 1;
select STDERR; $| = 1;
$exit = 0;

sub complain($@){
   printf STDERR "%s\n", join ": ", @_[1..$#_];
   $exit = $_[0] if $exit < $_[0];
};

sub readfile($){
   unless( open FILE, "<$_[0]" ){ complain 2, $0, @_, $! }
   else{ @input = <FILE>;
         close FILE;
         print "$0: @{[@input+0]} lines read from $_[0]\n";
         splice @lines, $dotline + 1, 0, @input;
         $dotline += @input;
   };
};

sub writefile($){
   unless( open FILE, ">$_[0]" ){ complain 2, $0, @_, $! }
   else{ print FILE @lines;
         close FILE;
         $dotline = $#lines;
         print "$0: @{[@lines+0]} lines written to $_[0]\n";
   };
};

sub printcurr(){
   $dotline = 0 if $dotline < 0;
   $dotline = $#lines if $dotline > $#lines;
   if( $dotline >= 0 ){ printf "%6d  %s", $dotline, $lines[$dotline] }
                  else{ complain 1, $0, "no lines in file" };
};

%switch = (
   '$' => [0, 1, sub{ $dotline = $#lines }],
   '*' => [0, 0, sub{ $dotline = $_, printcurr for 0..$#lines }],
   '.' => [0, 1, sub{ }],
   '0' => [0, 1, sub{ $dotline = 0 }],
   '<' => [0, 1, sub{ $dotline-- }],
   '>' => [0, 1, sub{ $dotline++ }],
   'a' => [1, 1, sub{ splice @lines, ++$dotline, 0, "$operand\n" }],
   'd' => [0, 1, sub{ splice @lines, $dotline, 1 if @lines }],
   'i' => [1, 1, sub{ $dotline = 0 if $dotline < 0;
                      splice @lines, $dotline, 0, "$operand\n" }],
   'r' => [1, 0, sub{ readfile $operand }],
   'w' => [1, 0, sub{ writefile $operand }],
);

while( @ARGV and $ARGV[0] =~ s/^-// and $opt = shift @ARGV ){
   $optecho = $opt =~ s/e//g;
   $optsilent = $opt =~ s/s//g;
   complain 1, "Usage", "$0 [-es] [filename...]" if $opt and ! $usage;
   $usage = 1;
};

@lines = <> if @ARGV;
$dotline = $#lines;
print "$0: @{[@lines+0]} lines read.\n";

for(;;){
   print "$0: " unless $optsilent;
   last unless defined( $command = <STDIN> );
   print $command if $optecho;
   chomp $command;
   ($key, $operand) = $command =~ m/(.)(.*)/;
   if( length $key and defined( $sub = $switch{$key} )
                   and ($sub->[0] or 0 == length $operand) ){
      &{$sub->[2]};
      printcurr if $sub->[1];
   }else{
      complain 1, $0, $command, "invalid command";
   };
};

print "^D" if $optecho;
print "\n";
exit $exit;

