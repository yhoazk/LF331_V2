# Inotify

this is a linux utility to register a call to let know the kernel that we are
interested in any operation that migth be performed in a specified file.
For each file a cookie is created and a callback registered.

In the example is printing the operation being performed to the file. The demo
needs two terminals, in one the executable is running an the operations are 
printed int the stdio. In a different terminal the we can create files or stuff
which will be registered by our application.

e.g.:
```
# In terminal x
touch test_file
./inotify_demo test_file
```

In a separate terminal:

```
touch test_file
cat > test_file

```
