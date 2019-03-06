#!/usr/env/python

def test_connection(hostname='poskbk.com', port=8989, timeout=10):
    """Test that the login prompt shows up if you try to connect.

    When spinning up KBK, the login prompt should show up on successful
    connection.  The prompt reads: By what name do you wish to be known?
    If we get that in our messages at some point, then we'll know we're
    successfully able to connect to the service.

    Args:
        hostname: url/ip of the host to connect to
        port: port number to attempt connectoin on
        timeout: timeout value before we'll fail the connection.
    """

    start = time.time()
    now = time.time()
    elapsed = 0
    connected = False
    try:
        tn = telnetlib.Telnet(hostname, port=port, timeout=timeout)
        # We'll give it up to a whole second to test.  This should happen
        # almost instantaneously.
        while elapsed < 1:
            msg = True
            while msg:
                msg = tn.read_eager()
                if msg:
                    msg = codecs.decode(msg)
                    if 'By what name do you wish to be known?' in msg:
                        connected = True
            elapsed = time.time() - start
        tn.close()
    # Super general exception, because we don't care *why* we can't connect
    # just that we can't.
    except:
        pass
    return connected

if __name__ == "__main__":
    test_connection()
