// Decompiled by DJ v3.12.12.96 Copyright 2011 Atanas Neshkov  Date: 2017/4/20 15:23:18
// Home Page: http://members.fortunecity.com/neshkov/dj.html  http://www.neshkov.com/dj.html - Check often for new version!
// Decompiler options: packimports(3) 

package org.lionsoul.ip2region;


// Referenced classes of package org.lionsoul.ip2region:
//            Util

public class HeaderBlock
{

    public HeaderBlock(long l, int i)
    {
        indexStartIp = l;
        indexPtr = i;
    }

    public long getIndexStartIp()
    {
        return indexStartIp;
    }

    public HeaderBlock setIndexStartIp(long l)
    {
        indexStartIp = l;
        return this;
    }

    public int getIndexPtr()
    {
        return indexPtr;
    }

    public HeaderBlock setIndexPtr(int i)
    {
        indexPtr = i;
        return this;
    }

    public byte[] getBytes()
    {
        byte abyte0[] = new byte[8];
        Util.writeIntLong(abyte0, 0, indexStartIp);
        Util.writeIntLong(abyte0, 4, indexPtr);
        return abyte0;
    }

    private long indexStartIp;
    private int indexPtr;
}
