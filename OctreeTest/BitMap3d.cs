﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace OctreeTest
{
    public class BitMap3d
    {
        public const byte WHITE = 255;
        public const byte BLACK = 0;
        public byte[] data;
        public int width;
        public int height;
        public int depth;
        public BitMap3d(int width, int height, int depth, byte v)
        {
            this.width = width;
            this.height = height;
            this.depth = depth;
            data = new byte[width * height * depth];
            for (int i = 0; i < width * height * depth; i++)
                data[i] = v;
        }
        public BitMap3d(byte[] data, int width, int height, int depth)
        {
            this.data = data;
            this.width = width;
            this.height = height;
            this.depth = depth;
        }
        public void SetPixel(int x, int y, int z, byte v)
        {
            data[x + y * width + z * width * height] = v;
        }
        public byte GetPixel(int x, int y, int z)
        {
            return data[x + y * width + z * width * height];
        }
        public void ReadRaw(string path)
        {
            FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read);
            fs.Read(data, 0, width * height * depth);
            fs.Close();
        }
        public void SaveRaw(string path)
        {
            FileStream fs = new FileStream(path, FileMode.OpenOrCreate, FileAccess.Write);
            fs.Write(data, 0, data.Length);
            fs.Close();
        }
        public static BitMap3d CreateSampleTedVolume(int width)
        {
            BitMap3d image = new BitMap3d(width, width, width, BitMap3d.BLACK);
            byte[] data = image.data;
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    for (int k = 0; k < width; k++)
                    {
                        if (i + j + k <= width - 2 && i >= 3 && j >= 3 && k >= 3)
                        {
                            data[i * width * width + j * width + k] = BitMap3d.WHITE;
                        }
                        else
                        {
                            data[i * width * width + j * width + k] = BitMap3d.BLACK;
                        }
                    }
                }
            }
            return image;
        }
        public static BitMap3d CreateSampleEngineVolume()
        {
            BitMap3d image = new BitMap3d(256, 256, 128, BitMap3d.BLACK);
            image.ReadRaw("D://VTKproj//engine.raw");
            byte[] data = image.data;
            for (int i = 0; i < data.Length; i++)
            {
                if (data[i] >= 64 && data[i] <= 255)
                    data[i] = BitMap3d.WHITE;
                else
                    data[i] = BitMap3d.BLACK;
            }
            return image;
        }
    }
}
