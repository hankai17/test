// Decompiled by DJ v3.12.12.96 Copyright 2011 Atanas Neshkov  Date: 2017/4/20 15:22:45
// Home Page: http://members.fortunecity.com/neshkov/dj.html  http://www.neshkov.com/dj.html - Check often for new version!
// Decompiler options: packimports(3) 

package org.lionsoul.ip2region;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.*;

// Referenced classes of package org.lionsoul.ip2region:
//            IndexBlock, HeaderBlock, DataBlock, DbConfig, 
//            DbMakerConfigException, Util

public class DbMaker
{

    public DbMaker(DbConfig dbconfig, String s, String s1)
        throws DbMakerConfigException, IOException
    {
        globalRegionFile = null;
        globalRegionMap = null;
        regionPtrPool = null;
        dbConfig = dbconfig;
        ipSrcFile = new File(s);
        globalRegionMap = new HashMap();
        regionPtrPool = new HashMap();
        if(s1 != null)
            globalRegionFile = new File(s1);
        if(!ipSrcFile.exists())
            throw new IOException((new StringBuilder()).append("Error: Invalid file path ").append(s).toString());
        else
            return;
    }

    private void initDbFile(RandomAccessFile randomaccessfile)
        throws IOException
    {
        randomaccessfile.seek(0L);
        randomaccessfile.write(new byte[8]);
        randomaccessfile.write(new byte[dbConfig.getTotalHeaderSize()]);
        headerPool = new LinkedList();
        indexPool = new LinkedList();
    }

    public void make(String s)
        throws IOException
    {
        if(globalRegionFile != null)
        {
            System.out.println("+-Try to load the global region data ...");
            BufferedReader bufferedreader = new BufferedReader(new FileReader(globalRegionFile));
            Object obj = null;
            do
            {
                String s1;
                if((s1 = bufferedreader.readLine()) == null)
                    break;
                String as[] = s1.split(",");
                if(as.length == 5)
                    globalRegionMap.put(as[2], Integer.valueOf(as[0]));
            } while(true);
            bufferedreader.close();
            System.out.println("|--[Ok]");
        }
        BufferedReader bufferedreader1 = new BufferedReader(new FileReader(ipSrcFile));
        RandomAccessFile randomaccessfile = new RandomAccessFile(s, "rw");
        initDbFile(randomaccessfile);
        System.out.println("+-Db file initialized.");
        System.out.println("+-Try to write the data blocks ... ");
        Object obj1 = null;
        do
        {
            String s2;
            if((s2 = bufferedreader1.readLine()) == null)
                break;
            s2 = s2.trim();
            if(s2.length() != 0 && s2.charAt(0) != '#')
            {
                int i = 0;
                int j = 0;
                if((j = s2.indexOf('|', i + 1)) != -1)
                {
                    String s3 = s2.substring(i, j);
                    i = j + 1;
                    if((j = s2.indexOf('|', i + 1)) != -1)
                    {
                        String s4 = s2.substring(i, j);
                        i = j + 1;
                        String s5 = s2.substring(i);
                        System.out.println((new StringBuilder()).append("+-Try to process item ").append(s2).toString());
                        addDataBlock(randomaccessfile, s3, s4, s5);
                        System.out.println("|--[Ok]");
                    }
                }
            }
        } while(true);
        System.out.println("|--Data block flushed!");
        System.out.println((new StringBuilder()).append("|--Data file pointer: ").append(randomaccessfile.getFilePointer()).append("\n").toString());
        System.out.println("+-Try to write index blocks ... ");
        IndexBlock indexblock = null;
        Object obj2 = null;
        indexblock = (IndexBlock)indexPool.getFirst();
        long l = indexblock.getStartIp();
        long l1 = randomaccessfile.getFilePointer();
        headerPool.add(new HeaderBlock(l, (int)l1));
        int k = IndexBlock.getIndexBlockLength();
        int i1 = 0;
        int j1 = dbConfig.getIndexBlockSize() / k - 1;
        IndexBlock indexblock1;
        for(Iterator iterator = indexPool.iterator(); iterator.hasNext(); randomaccessfile.write(indexblock1.getBytes()))
        {
            indexblock1 = (IndexBlock)iterator.next();
            if(++i1 >= j1)
            {
                HeaderBlock headerblock = new HeaderBlock(indexblock1.getStartIp(), (int)randomaccessfile.getFilePointer());
                headerPool.add(headerblock);
                i1 = 0;
            }
        }

        if(i1 > 0)
        {
            IndexBlock indexblock2 = (IndexBlock)indexPool.getLast();
            HeaderBlock headerblock1 = new HeaderBlock(indexblock2.getStartIp(), (int)randomaccessfile.getFilePointer() - IndexBlock.getIndexBlockLength());
            headerPool.add(headerblock1);
        }
        long l2 = randomaccessfile.getFilePointer();
        System.out.println("|--[Ok]");
        System.out.println("+-Try to write the super blocks ... ");
        randomaccessfile.seek(0L);
        byte abyte0[] = new byte[8];
        Util.writeIntLong(abyte0, 0, l1);
        Util.writeIntLong(abyte0, 4, l2 - (long)k);
        randomaccessfile.write(abyte0);
        System.out.println("|--[Ok]");
        System.out.println("+-Try to write the header blocks ... ");
        HeaderBlock headerblock2;
        for(Iterator iterator1 = headerPool.iterator(); iterator1.hasNext(); randomaccessfile.write(headerblock2.getBytes()))
            headerblock2 = (HeaderBlock)iterator1.next();

        System.out.println("+-Try to write the copyright and release date info ... ");
        randomaccessfile.seek(randomaccessfile.length());
        Calendar calendar = Calendar.getInstance();
        SimpleDateFormat simpledateformat = new SimpleDateFormat("yyyy/MM/dd");
        String s6 = (new StringBuilder()).append("Created by lionsoul at ").append(simpledateformat.format(calendar.getTime())).toString();
        randomaccessfile.write((int)(calendar.getTime().getTime() / 1000L));
        randomaccessfile.write(s6.getBytes());
        System.out.println("|--[Ok]");
        bufferedreader1.close();
        randomaccessfile.close();
    }

    private void addDataBlock(RandomAccessFile randomaccessfile, String s, String s1, String s2)
    {
        try
        {
            byte abyte0[] = s2.getBytes("UTF-8");
            int i = 0;
            if(regionPtrPool.containsKey(s2))
            {
                DataBlock datablock = (DataBlock)regionPtrPool.get(s2);
                i = datablock.getDataPtr();
                System.out.println((new StringBuilder()).append("dataPtr: ").append(i).append(", region: ").append(s2).toString());
            } else
            {
                byte abyte1[] = new byte[4];
                int j = getCityId(s2);
                Util.writeIntLong(abyte1, 0, j);
                i = (int)randomaccessfile.getFilePointer();
                randomaccessfile.write(abyte1);
                randomaccessfile.write(abyte0);
                regionPtrPool.put(s2, new DataBlock(j, s2, i));
            }
            IndexBlock indexblock = new IndexBlock(Util.ip2long(s), Util.ip2long(s1), i, abyte0.length + 4);
            indexPool.add(indexblock);
        }
        catch(UnsupportedEncodingException unsupportedencodingexception)
        {
            unsupportedencodingexception.printStackTrace();
        }
        catch(IOException ioexception)
        {
            ioexception.printStackTrace();
        }
    }

    public int getCityId(String s)
    {
        String as[] = s.split("\\|");
        if(as.length != 5)
            return 0;
        Object obj = null;
        Object obj1 = null;
        for(int i = 3; i >= 0; i--)
            if(!as[i].equals("0"))
            {
                String s1;
                if(i == 3 && as[i].indexOf("\u7701\u76F4\u8F96\u53BF\u7EA7") > -1)
                    s1 = (new StringBuilder()).append(as[2]).append(as[3]).toString();
                else
                    s1 = as[i];
                Integer integer = (Integer)globalRegionMap.get(s1);
                if(integer == null)
                    return 0;
                else
                    return integer.intValue();
            }

        return 0;
    }

    public DbConfig getDbConfig()
    {
        return dbConfig;
    }

    public DbMaker setDbConfig(DbConfig dbconfig)
    {
        dbConfig = dbconfig;
        return this;
    }

    public File getIpSrcFile()
    {
        return ipSrcFile;
    }

    public DbMaker setIpSrcFile(File file)
    {
        ipSrcFile = file;
        return this;
    }

    public static void main(String args[])
    {
        String s = "./data/";
        String args1[] = {
            null, null
        };
        for(int i = 0; i < args.length; i++)
        {
            if(args[i].equals("-src"))
            {
                args1[0] = args[++i];
                continue;
            }
            if(args[i].equals("-region"))
            {
                args1[1] = args[++i];
                continue;
            }
            if(args[i].equals("-dst"))
                s = args[++i];
        }

        for(int j = 0; j < args1.length; j++)
            if(args1[j] == null)
            {
                System.out.println("Usage: java -jar dbMaker.jar -src [source text file path] -region [global region file path]");
                System.out.println("eg: java -jar dbMaker.jar -src ./data/ip.merge.txt -region ./data/origin/global_region.csv");
                System.exit(0);
            }

        if(!s.endsWith("/"))
            s = (new StringBuilder()).append(s).append("/").toString();
        try
        {
            DbConfig dbconfig = new DbConfig();
            DbMaker dbmaker = new DbMaker(dbconfig, args1[0], args1[1]);
            dbmaker.make((new StringBuilder()).append(s).append("ip2region.db").toString());
        }
        catch(DbMakerConfigException dbmakerconfigexception)
        {
            dbmakerconfigexception.printStackTrace();
        }
        catch(IOException ioexception)
        {
            ioexception.printStackTrace();
        }
    }

    private DbConfig dbConfig;
    private File ipSrcFile;
    private LinkedList indexPool;
    private LinkedList headerPool;
    private File globalRegionFile;
    private HashMap globalRegionMap;
    private HashMap regionPtrPool;
}
