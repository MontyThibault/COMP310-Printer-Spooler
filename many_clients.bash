# Print every filename in this directory.
for i in $(ls); do echo $i | ./client; done;
