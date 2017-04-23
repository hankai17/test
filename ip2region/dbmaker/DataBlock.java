// Decompiled by DJ v3.12.12.96 Copyright 2011 Atanas Neshkov  Date: 2017/4/20 15:22:05
// Home Page: http://members.fortunecity.com/neshkov/dj.html  http://www.neshkov.com/dj.html - Check often for new version!
// Decompiler options: packimports(3) 

package org.lionsoul.ip2region;


public class DataBlock
{

    public DataBlock(int i, String s, int j)
    {
        city_id = i;
        region = s;
        dataPtr = j;
    }

    public DataBlock(int i, String s)
    {
        this(i, s, 0);
    }

    public int getCityId()
    {
        return city_id;
    }

    public DataBlock setCityId(int i)
    {
        city_id = i;
        return this;
    }

    public String getRegion()
    {
        return region;
    }

    public DataBlock setRegion(String s)
    {
        region = s;
        return this;
    }

    public int getDataPtr()
    {
        return dataPtr;
    }

    public DataBlock setDataPtr(int i)
    {
        dataPtr = i;
        return this;
    }

    public String toString()
    {
        StringBuilder stringbuilder = new StringBuilder();
        stringbuilder.append(city_id).append('|').append(region).append('|').append(dataPtr);
        return stringbuilder.toString();
    }

    private int city_id;
    private String region;
    private int dataPtr;
}
