use 5.012004;
use ExtUtils::MakeMaker;

# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.

my $CC = 'g++';

WriteMakefile(
	NAME              => 'SUP::XTL',
	VERSION_FROM      => 'lib/SUP/XTL.pm', # finds $VERSION
	PREREQ_PM         => {}, # e.g., Module::Name => 1.1
	CC                => $CC,
	LD                => '$(CC)',
	($] >= 5.005 ?     ## Add these new keywords supported since 5.005
		(ABSTRACT_FROM  => 'lib/SUP/XTL.pm', # retrieve abstract from module
		 AUTHOR         => 'A. U. Thor <dnikolaev@>') : ()),
	LIBS              => [''], # e.g., '-lm'
	MYEXTLIB          => '/home/dnikolaev/Dropbox/Projects/cxx/xtl/build/lib/libxtl-common-st.a',
	DEFINE            => '', # e.g., '-DHAVE_SOMETHING'
	INC               => '-I. -I../../lib', # e.g., '-I. -I/usr/include/other'
	# Un-comment this if you add C files to link with later:
	OBJECT            => '$(O_FILES)', # link all the C files too
);

