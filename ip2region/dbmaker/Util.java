// Decompiled by DJ v3.12.12.96 Copyright 2011 Atanas Neshkov  Date: 2017/4/20 15:23:39
// Home Page: http://members.fortunecity.com/neshkov/dj.html  http://www.neshkov.com/dj.html - Check often for new version!
// Decompiler options: packimports(3) 

package org.lionsoul.ip2region;


public class Util
{

    public Util()
    {
    }

    public static void write(byte abyte0[], int i, long l, int j)
    {
        for(int k = 0; k < j; k++)
            abyte0[i++] = (byte)(int)(l >>> 8 * k & 255L);

    }

    public static void writeIntLong(byte abyte0[], int i, long l)
    {
        abyte0[i++] = (byte)(int)(l >> 0 & 255L);
        abyte0[i++] = (byte)(int)(l >> 8 & 255L);
        abyte0[i++] = (byte)(int)(l >> 16 & 255L);
        abyte0[i] = (byte)(int)(l >> 24 & 255L);
    }

    public static long getIntLong(byte abyte0[], int i)
    {
        return (long)abyte0[i++] & 255L | (long)(abyte0[i++] << 8) & 65280L | (long)(abyte0[i++] << 16) & 0xff0000L | (long)(abyte0[i] << 24) & 0xff000000L;
    }

    public static int getInt3(byte abyte0[], int i)
    {
        return abyte0[i++] & 0xff | abyte0[i++] & 0xff00 | abyte0[i] & 0xff0000;
    }

    public static int getInt2(byte abyte0[], int i)
    {
        return abyte0[i++] & 0xff | abyte0[i] & 0xff00;
    }

    public static int getInt1(byte abyte0[], int i)
    {
        return abyte0[i] & 0xff;
    }

    public static long ip2long(String s)
    {
        String as[] = s.split("\\.");
        if(as.length != 4)
        {
            return 0L;
        } else
        {
            int i = Integer.valueOf(as[0]).intValue() << 24 & 0xff000000;
            int j = Integer.valueOf(as[1]).intValue() << 16 & 0xff0000;
            int k = Integer.valueOf(as[2]).intValue() << 8 & 0xff00;
            int l = Integer.valueOf(as[3]).intValue() << 0 & 0xff;
            return (long)(i | j | k | l) & 0xffffffffL;
        }
    }

    public static String long2ip(long l)
    {
        StringBuilder stringbuilder = new StringBuilder();
        stringbuilder.append(l >> 24 & 255L).append('.').append(l >> 16 & 255L).append('.').append(l >> 8 & 255L).append('.').append(l >> 0 & 255L);
        return stringbuilder.toString();
    }

    public static boolean isIpAddress(String s)
    {
        String as[] = s.split("\\.");
        if(as.length != 4)
            return false;
        String as1[] = as;
        int i = as1.length;
        for(int j = 0; j < i; j++)
        {
            String s1 = as1[j];
            if(s1.length() > 3)
                return false;
            int k = Integer.valueOf(s1).intValue();
            if(k > 255)
                return false;
        }

        return true;
    }
}
