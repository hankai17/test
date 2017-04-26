// Decompiled by DJ v3.12.12.96 Copyright 2011 Atanas Neshkov  Date: 2017/4/20 15:23:08
// Home Page: http://members.fortunecity.com/neshkov/dj.html  http://www.neshkov.com/dj.html - Check often for new version!
// Decompiler options: packimports(3) 

package org.lionsoul.ip2region;

import java.io.*;

// Referenced classes of package org.lionsoul.ip2region:
//            DataBlock, IndexBlock, Util, DbConfig

public class DbSearcher
{

    public DbSearcher(DbConfig dbconfig, String s)
        throws FileNotFoundException
    {
        dbConfig = null;
        raf = null;
        HeaderSip = null;
        HeaderPtr = null;
        firstIndexPtr = 0L;
        lastIndexPtr = 0L;
        totalIndexBlocks = 0;
        dbBinStr = null;
        dbConfig = dbconfig;
        raf = new RandomAccessFile(s, "r");
    }

    public DataBlock memorySearch(long l)
        throws IOException
    {
        long l3;
label0:
        {
            int i = IndexBlock.getIndexBlockLength();
            if(dbBinStr == null)
            {
                dbBinStr = new byte[(int)raf.length()];
                raf.seek(0L);
                raf.readFully(dbBinStr, 0, dbBinStr.length);
                firstIndexPtr = Util.getIntLong(dbBinStr, 0);
                lastIndexPtr = Util.getIntLong(dbBinStr, 4);
                totalIndexBlocks = (int)((lastIndexPtr - firstIndexPtr) / (long)i) + 1;
            }
            int j = 0;
            int k = totalIndexBlocks;
            l3 = 0L;
            int k1;
            do
            {
                if(j > k)
                    break label0;
                int i1 = j + k >> 1;
                k1 = (int)(firstIndexPtr + (long)(i1 * i));
                long l1 = Util.getIntLong(dbBinStr, k1);
                if(l < l1)
                {
                    k = i1 - 1;
                    continue;
                }
                long l2 = Util.getIntLong(dbBinStr, k1 + 4);
                if(l <= l2)
                    break;
                j = i1 + 1;
            } while(true);
            l3 = Util.getIntLong(dbBinStr, k1 + 8);
        }
        if(l3 == 0L)
        {
            return null;
        } else
        {
            int j1 = (int)(l3 >> 24 & 255L);
            int i2 = (int)(l3 & 0xffffffL);
            int j2 = (int)Util.getIntLong(dbBinStr, i2);
            String s = new String(dbBinStr, i2 + 4, j1 - 4, "UTF-8");
            return new DataBlock(j2, s, i2);
        }
    }

    public DataBlock memorySearch(String s)
        throws IOException
    {
        return memorySearch(Util.ip2long(s));
    }

    public DataBlock getByIndexPtr(long l)
        throws IOException
    {
        raf.seek(l);
        byte abyte0[] = new byte[12];
        raf.readFully(abyte0, 0, abyte0.length);
        long l1 = Util.getIntLong(abyte0, 8);
        int i = (int)(l1 >> 24 & 255L);
        int j = (int)(l1 & 0xffffffL);
        raf.seek(j);
        byte abyte1[] = new byte[i];
        raf.readFully(abyte1, 0, abyte1.length);
        int k = (int)Util.getIntLong(abyte1, 0);
        String s = new String(abyte1, 4, abyte1.length - 4, "UTF-8");
        return new DataBlock(k, s, j);
    }

    public DataBlock btreeSearch(long l)
        throws IOException
    {
        long l5;
label0:
        {
            if(HeaderSip == null)
            {
                raf.seek(8L);
                byte abyte0[] = new byte[4096];
                raf.readFully(abyte0, 0, abyte0.length);
                int j = abyte0.length >> 3;
                int i1 = 0;
                HeaderSip = new long[j];
                HeaderPtr = new int[j];
                int i3 = 0;
                do
                {
                    if(i3 >= abyte0.length)
                        break;
                    long l1 = Util.getIntLong(abyte0, i3);
                    long l2 = Util.getIntLong(abyte0, i3 + 4);
                    if(l2 == 0L)
                        break;
                    HeaderSip[i1] = l1;
                    HeaderPtr[i1] = (int)l2;
                    i1++;
                    i3 += 8;
                } while(true);
                headerLength = i1;
            }
            if(l == HeaderSip[0])
                return getByIndexPtr(HeaderPtr[0]);
            if(l == HeaderSip[headerLength - 1])
                return getByIndexPtr(HeaderPtr[headerLength - 1]);
            int i = 0;
            int k = headerLength;
            int j1 = 0;
            int k1 = 0;
            do
            {
                if(i > k)
                    break;
                int i2 = i + k >> 1;
                if(l == HeaderSip[i2])
                {
                    if(i2 > 0)
                    {
                        j1 = HeaderPtr[i2 - 1];
                        k1 = HeaderPtr[i2];
                    } else
                    {
                        j1 = HeaderPtr[i2];
                        k1 = HeaderPtr[i2 + 1];
                    }
                    break;
                }
                if(l < HeaderSip[i2])
                {
                    if(i2 == 0)
                    {
                        j1 = HeaderPtr[i2];
                        k1 = HeaderPtr[i2 + 1];
                        break;
                    }
                    if(l > HeaderSip[i2 - 1])
                    {
                        j1 = HeaderPtr[i2 - 1];
                        k1 = HeaderPtr[i2];
                        break;
                    }
                    k = i2 - 1;
                    continue;
                }
                if(i2 == headerLength - 1)
                {
                    j1 = HeaderPtr[i2 - 1];
                    k1 = HeaderPtr[i2];
                    break;
                }
                if(l <= HeaderSip[i2 + 1])
                {
                    j1 = HeaderPtr[i2];
                    k1 = HeaderPtr[i2 + 1];
                    break;
                }
                i = i2 + 1;
            } while(true);
            if(j1 == 0)
                return null;
            int j2 = k1 - j1;
            int k2 = IndexBlock.getIndexBlockLength();
            byte abyte1[] = new byte[j2 + k2];
            raf.seek(j1);
            raf.readFully(abyte1, 0, abyte1.length);
            i = 0;
            k = j2 / k2;
            l5 = 0L;
            int i4;
            do
            {
                if(i > k)
                    break label0;
                int j3 = i + k >> 1;
                i4 = j3 * k2;
                long l3 = Util.getIntLong(abyte1, i4);
                if(l < l3)
                {
                    k = j3 - 1;
                    continue;
                }
                long l4 = Util.getIntLong(abyte1, i4 + 4);
                if(l <= l4)
                    break;
                i = j3 + 1;
            } while(true);
            l5 = Util.getIntLong(abyte1, i4 + 8);
        }
        if(l5 == 0L)
        {
            return null;
        } else
        {
            int k3 = (int)(l5 >> 24 & 255L);
            int j4 = (int)(l5 & 0xffffffL);
            raf.seek(j4);
            byte abyte2[] = new byte[k3];
            raf.readFully(abyte2, 0, abyte2.length);
            int k4 = (int)Util.getIntLong(abyte2, 0);
            String s = new String(abyte2, 4, abyte2.length - 4, "UTF-8");
            return new DataBlock(k4, s, j4);
        }
    }

    public DataBlock btreeSearch(String s)
        throws IOException
    {
        return btreeSearch(Util.ip2long(s));
    }

    public DataBlock binarySearch(long l)
        throws IOException
    {
        long l3;
label0:
        {
            int i = IndexBlock.getIndexBlockLength();
            if(totalIndexBlocks == 0)
            {
                raf.seek(0L);
                byte abyte0[] = new byte[8];
                raf.readFully(abyte0, 0, abyte0.length);
                firstIndexPtr = Util.getIntLong(abyte0, 0);
                lastIndexPtr = Util.getIntLong(abyte0, 4);
                totalIndexBlocks = (int)((lastIndexPtr - firstIndexPtr) / (long)i) + 1;
            }
            int j = 0;
            int k = totalIndexBlocks;
            byte abyte1[] = new byte[i];
            l3 = 0L;
            do
            {
                if(j > k)
                    break label0;
                int i1 = j + k >> 1;
                raf.seek(firstIndexPtr + (long)(i1 * i));
                raf.readFully(abyte1, 0, abyte1.length);
                long l1 = Util.getIntLong(abyte1, 0);
                if(l < l1)
                {
                    k = i1 - 1;
                    continue;
                }
                long l2 = Util.getIntLong(abyte1, 4);
                if(l <= l2)
                    break;
                j = i1 + 1;
            } while(true);
            l3 = Util.getIntLong(abyte1, 8);
        }
        if(l3 == 0L)
        {
            return null;
        } else
        {
            int j1 = (int)(l3 >> 24 & 255L);
            int k1 = (int)(l3 & 0xffffffL);
            raf.seek(k1);
            byte abyte2[] = new byte[j1];
            raf.readFully(abyte2, 0, abyte2.length);
            int i2 = (int)Util.getIntLong(abyte2, 0);
            String s = new String(abyte2, 4, abyte2.length - 4, "UTF-8");
            return new DataBlock(i2, s, k1);
        }
    }

    public DataBlock binarySearch(String s)
        throws IOException
    {
        return binarySearch(Util.ip2long(s));
    }

    public DbConfig getDbConfig()
    {
        return dbConfig;
    }

    public void close()
        throws IOException
    {
        HeaderSip = null;
        HeaderPtr = null;
        dbBinStr = null;
        raf.close();
    }

    public static final int BTREE_ALGORITHM = 1;
    public static final int BINARY_ALGORITHM = 2;
    public static final int MEMORY_ALGORITYM = 3;
    private DbConfig dbConfig;
    private RandomAccessFile raf;
    private long HeaderSip[];
    private int HeaderPtr[];
    private int headerLength;
    private long firstIndexPtr;
    private long lastIndexPtr;
    private int totalIndexBlocks;
    private byte dbBinStr[];
}
