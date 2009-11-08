#!/usr/bin/env perl

print "Hello World!\n";
print "Hello
	World\n";
print 'Hello\nWorld\n';
print 234;
my $x = 23;
my $y = 'abc';

print $y;
print $x;
print "


";

print "Array Time\n";

my @one = (1,2,3,4,5);
my @two = ('a','b','c','d');
my @thr = (1,'2',3,'4',5);

print @one,"\n";
print @two,"\n";
print @thr,"\n";
print @one[1..3],"\n";
print $two[3],"\n";
print @ARGV,"\n";

print "Hashes!!!!\n";

my %h = (uno=>"one",dos=>"two",tres=>"three",quatro=>"four");
print keys %h,"\n";
print sort (keys %h),"\n";

    my $variables = {
        scalar  =>  { 
                     description => "single item",
                     sigil => '$',
                    },
        array   =>  {
                     description => "ordered list of items",
                     sigil => '@',
                    },
        hash    =>  {
                     description => "key/value pairs",
                     sigil => '%',
                    },
    };


 print "Scalars begin with a $variables->{'scalar'}->{'sigil'}\n";

# print "LA LA LA " until 0;

open(IN,"txt") or die "Can't open txt: $!";
while(<IN>) {
	print $_;
}
