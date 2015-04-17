=begin

# apparently I cannot change my ulimit -c from within, it has to be done by
# a privileged process before cucumber begins.  This code therefore does
# not work and is commented out
require 'inline'

class SetCoreFileSize
	inline do |builder|
		builder.include '<sys/time.h>'
		builder.include '<sys/resource.h>'
		builder.c '
			int callSetRlimit(int coresize) {
				struct rlimit r;
				r.rlim_cur = coresize;
				r.rlim_max = coresize;
				int rc = setrlimit(RLIMIT_CORE, &r);
				return rc;
			} '
	end
end

coresize = 500000

rc = SetCoreFileSize.new.callSetRlimit(coresize)
if rc == 0
	puts "Creation of core files enabled with max size of #{coresize}"
else
	puts "Core file will not be created. setrlimit() failed with rc=#{rc}"
end
=end

