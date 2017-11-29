# uwsgi-reload-spinlock

uwsgi gracefull reload use pthread shutdown system.
unfortunately, pthread shutdown/join not work properly on alpine linux musl libc.
this plugin fix this problem.

## build

```
$ uwsgi --build-plugin https://github.com/afiram/uwsgi-reload-spinlock/
```
