// Decompiled by DJ v3.12.12.96 Copyright 2011 Atanas Neshkov  Date: 2017/4/20 15:22:33
// Home Page: http://members.fortunecity.com/neshkov/dj.html  http://www.neshkov.com/dj.html - Check often for new version!
// Decompiler options: packimports(3) 

package org.lionsoul.ip2region;


// Referenced classes of package org.lionsoul.ip2region:
//            DbMakerConfigException

public class DbConfig
{

    public DbConfig(int i)
        throws DbMakerConfigException
    {
        if(i % 8 != 0)
        {
            throw new DbMakerConfigException("totalHeaderSize must be times of 8");
        } else
        {
            totalHeaderSize = i;
            indexBlockSize = 4096;
            return;
        }
    }

    public DbConfig()
        throws DbMakerConfigException
    {
        this(8192);
    }

    public int getTotalHeaderSize()
    {
        return totalHeaderSize;
    }

    public DbConfig setTotalHeaderSize(int i)
    {
        totalHeaderSize = i;
        return this;
    }

    public int getIndexBlockSize()
    {
        return indexBlockSize;
    }

    public DbConfig setIndexBlockSize(int i)
    {
        indexBlockSize = i;
        return this;
    }

    private int totalHeaderSize;
    private int indexBlockSize;
}
