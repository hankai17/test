// Decompiled by DJ v3.12.12.96 Copyright 2011 Atanas Neshkov  Date: 2017/4/20 15:23:29
// Home Page: http://members.fortunecity.com/neshkov/dj.html  http://www.neshkov.com/dj.html - Check often for new version!
// Decompiler options: packimports(3) 

package org.lionsoul.ip2region;


// Referenced classes of package org.lionsoul.ip2region:
//            Util

public class IndexBlock
{

    public IndexBlock(long l, long l1, int i, int j)
    {
        startIp = l;
        endIp = l1;
        dataPtr = i;
        dataLen = j;
    }

    public long getStartIp()
    {
        return startIp;
    }

    public IndexBlock setStartIp(long l)
    {
        startIp = l;
        return this;
    }

    public long getEndIp()
    {
        return endIp;
    }

    public IndexBlock setEndIp(long l)
    {
        endIp = l;
        return this;
    }

    public int getDataPtr()
    {
        return dataPtr;
    }

    public IndexBlock setDataPtr(int i)
    {
        dataPtr = i;
        return this;
    }

    public int getDataLen()
    {
        return dataLen;
    }

    public IndexBlock setDataLen(int i)
    {
        dataLen = i;
        return this;
    }

    public static int getIndexBlockLength()
    {
        return LENGTH;
    }

    public byte[] getBytes()
    {
        byte abyte0[] = new byte[12];
        Util.writeIntLong(abyte0, 0, startIp);
        Util.writeIntLong(abyte0, 4, endIp);
        long l = (long)dataPtr | (long)(dataLen << 24) & 0xff000000L;
        Util.writeIntLong(abyte0, 8, l);
        return abyte0;
    }

    private static int LENGTH = 12;
    private long startIp;
    private long endIp;
    private int dataPtr;
    private int dataLen;

}
