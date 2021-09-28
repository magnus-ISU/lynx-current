This github repository is modified from version 2.9.0dev.9, which is the download of
lynx-cur.tar.gz as of 2021 Sep 28. The makefile installs under `/usr/local/bin/`,
`/usr/local/share/man/man1/`, and `/usr/local/etc/` for the default configuration,
at least on my arch linux.

This repository's master branch does not exactly track with Lynx's upstream release. See the named branches if you want that.

To build, you probably want (otherwise see INSTALLATION and files under doc/ )

    ./configure --with-ssl --with-gnutls --with-gnutls-openssl
    make
    sudo make install # Or doas, etc

Implemented changes from stock Lynx:

 - Supports XDG_BASE_DIRECTORY specification, configuration file is under `XDG_CONFIG_HOME/lynx/lynx.cfg` by default. Falls back to `$HOME/.lynx/lynx.cfg`.

Intended features to be added eventually:

 - Provide a seperate, better configuration by default (currently see the file `lazy_lynx.cfg`)
 - Allow numbering of links by letters rather than numbers (to follow links from the home row)
 - Other stuff? Open an issue

I will also try to keep this up to date with lynx releases, better than the repository
this is forked from.  Why lynx developers don't use git, though, is beyond me.

The original README seems out of date and less than useful, but it can be found as the README file.
