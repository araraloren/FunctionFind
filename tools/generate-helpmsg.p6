#!/usr/bin/env perl6

class X::GetOption::Simple is Exception {
	has $.msg handles <Str>;
	method message() {
		$.msg;
	}
}

class GetOption::Simple {
	has %.options;
	has @.args;
	# key = optional
	# value:
	# prefix '-' || '--'
	# argument '!' || '*' || '?' || '@' || '%' || '^'
	# value
	#

	method new(*%options) {
		self.bless(options => %options)!addMethod();
	}
	
	method !addMethod() {
		for %!options.kv -> $key, $value {

			my %value = %$value;

			my $method-name = $key;

			self.^add_method($key, my method { %!options{$key}<value> } );
			self.^compose();

			%!options{$key}<prefix> = %value<long> ?? '--' !! '-';
			%!options{$key}<long>:delete;

			if !defined(%value<value>) {
				given %value<argument> {
					when '@' {
						%!options{$key}<value> = ();
					}
					when '%' {
						%!options{$key}<value> = %();
					}
					when '^' {
						%!options{$key}<value> = False;
					}
					default {
						next;
					}
				}
			}
		}
		self;
	}

	method parse(@!args = @*ARGS) returns Array {
		my $optional;
		
		while +@!args {
			$optional = @!args.shift;
			given $optional {
				when /^ '--' $<optname> = (.*) / {
					self.processOptional(~$<optname>, '--');
				}
				when /^ '-' $<optname> = (.*)/ {
					self.processOptional(~$<optname>, '-');
				}
				default {
					last;
				}
			}
		}

		self!check();
		
		return @!args;
	}

	method processOptional(Str $name, Str $prefix) {
		if !defined(%!options{$name}) || %!options{$name}<prefix> ne $prefix {
			X::GetOption::Simple.new(msg => $name ~ ' is not recognized.').throw;
		}

		if %!options{$name}<argument> ~~ /'!' | '@' | '*' | '%'/ && +@!args == 0 {
			X::GetOption::Simple.new(msg => $name ~ ' need argument.').throw;
		}
		
		given %!options{$name}<argument> {
			when '!' | '*' {
				%!options{$name}<value> = @!args.shift;
			}
			when '?' {
				if +@!args > 0 {
					%!options{$name}<value> =  @!args.shift if @!args[* - 1] !~~ /^\-||\-\-/;
				}
			}
			when '@' {
				%!options{$name}<value>.push: @!args.shift;
			}
			when '%' {
				if @!args.shift ~~ /^ $<key> = (<-[\=\>]>*) \=\> $<value> = (.*) / {
					%!options{$name}<value>{$<key>} = $<value>;
				} else {
					X::GetOption::Simple.new(msg => $name ~ ' argument error.').throw;
				} 
			}
			when '^' {
				%!options{$name}<value> = True;
			}
			default {
				X::GetOption::Simple.new(msg => $name ~ ' options argument error, should be !*@%^');
			}
		}
	}

	method has(Str $name) {
		return defined(%!options{$name}<value>);
	}

	method !check() {
		for %!options.kv -> $key, $value {
			if %($value)<argument> eq '!' && !defined(%($value)<value>) {
				X::GetOption::Simple.new(msg => 'need optional ' ~ %($value)<prefix> ~ $key ~ '.').throw;
			}
		}
	}
}

my %options = (
	design => %(:long, argument => '!'),
	out => %(:long, argument => '!'),
);

my $getopt = GetOption::Simple.new(|%options);

$getopt.parse();

my ($design, $out) = ($getopt.design(), $getopt.out());

my $ofh = $out.IO.open(:w);

for $design.IO.lines -> $line is copy {
	$line ~~ s:g/ '"' //;
	$ofh.put: '"' ~ $line ~ "\\n\"\t\\";
}

$ofh.close();




