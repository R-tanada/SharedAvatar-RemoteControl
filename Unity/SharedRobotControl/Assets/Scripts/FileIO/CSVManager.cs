using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System.Text;

public class CSVManager
{
    public List<int> IntListReader(string readPath, string fileName)
    {
        /// <summary>
        /// List<int>形式のデータを読み込む
        /// 
        /// Parameter
        /// ----------
        /// readPath: string
        ///     読み込むパス
        /// 
        /// fileName: string
        ///     ファイル名．省略可能
        /// 
        /// </summary>
        List<int> data = new List<int>();
        string filePath = Application.dataPath + readPath + fileName + ".csv";
        using (StreamReader streamReader = new StreamReader(filePath, Encoding.UTF8))
        {
            // ヘッダーを読み飛ばす
            streamReader.ReadLine();

            while (!streamReader.EndOfStream)
            {
                string line = streamReader.ReadLine();
                string[] dat = line.Split(',');
                data.Add(int.Parse(dat[0]));
            }
            return data;
        }
    }
    public List<Vector3> Vector3ListReader(string readPath, string fileName)
    {
        /// <summary>
        /// List<Vector3>形式のデータを読み込む
        /// 
        /// Parameter
        /// ----------
        /// readPath: string
        ///     読み込むパス
        /// 
        /// fileName: string
        ///     ファイル名．省略可能
        /// 
        /// </summary>

        List<Vector3> data = new List<Vector3>();
        string filePath = Application.dataPath + readPath + fileName + ".csv";
        using (StreamReader streamReader = new StreamReader(filePath, Encoding.UTF8))
        {
            // ヘッダーを読み飛ばす
            streamReader.ReadLine();

            while (!streamReader.EndOfStream)
            {
                string line = streamReader.ReadLine();
                string[] dat = line.Split(',');
                data.Add(new Vector3(
                    float.Parse(dat[0]),
                    float.Parse(dat[1]),
                    float.Parse(dat[2])));
            }
            return data;
        }
    }

    public void IntListWriter(List<int> dat, string path, string fileName, string header)
    {
        /// <summary>
        /// 
        /// List<int>形式のデータをCSVに書き出す
        /// 
        /// dat: int
        ///     書き出すデータ
        /// path: string
        ///     書き出すパス．ファイル名まで含めて指定可能
        /// fileName: string
        ///     ファイル名
        /// header; string
        ///     ヘッダー
        /// </summary>

        SafeCreateDirectory(Application.dataPath + path);

        StreamWriter sw;
        FileInfo fi;
        fi = new FileInfo(Application.dataPath + path + fileName + ".csv");
        sw = fi.AppendText();

        sw.WriteLine(header);
        foreach (int d in dat)
            sw.WriteLine(d);

        sw.Flush();
        sw.Close();
    }

    public void FloatListWriter(List<float> dat, string path, string fileName, string header)
    {
        /// <summary>
        /// 
        /// List<float>形式のデータをCSVに書き出す
        /// 
        /// dat: float
        ///     書き出すデータ
        /// path: string
        ///     書き出すパス．ファイル名まで含めて指定可能
        /// fileName: string
        ///     ファイル名
        /// header; string
        ///     ヘッダー
        /// </summary>

        SafeCreateDirectory(Application.dataPath + path);

        StreamWriter sw;
        FileInfo fi;
        fi = new FileInfo(Application.dataPath + path + fileName + ".csv");
        sw = fi.AppendText();

        sw.WriteLine(header);
        foreach (float d in dat)
            sw.WriteLine(d);

        sw.Flush();
        sw.Close();
    }

    public void Vector3ListWriter(List<Vector3> dat, string path, string fileName)
    {
        /// <summary>
        /// 
        /// List<Vector3>形式のデータをCSVに書き出す
        /// 
        /// param name="dat": Vector3
        ///     書き出すデータ
        /// param name="path": string
        ///     書き出すパス．ファイル名まで含めて指定可能
        /// param name="fileName": string
        ///     ファイル名
        /// </summary>

        SafeCreateDirectory(Application.dataPath + path);

        StreamWriter sw;
        FileInfo fi;
        fi = new FileInfo(Application.dataPath + path + fileName + ".csv");
        sw = fi.AppendText();

        sw.WriteLine("x,y,z");
        foreach (Vector3 d in dat)
        {
            sw.WriteLine(d.x + "," + d.y + "," + d.z);
        }
        sw.Flush();
        sw.Close();
    }

    /*
    public void MotionDataWriter(Dictionary<string, List<MotionRecorder.MotionData>> dat, string path, string fileName)
    {
        /// <summary>
        /// Motionrecorder.MotionDataをCSVに書き出す
        /// 
        /// Parameter
        /// ----------
        /// dat: Dictionary<string, List<MotionRecorder.MotionData>>
        ///     モーションデータ
        /// path: string
        ///     データの書き出しパス
        /// fileName: string
        ///     ファイル名
        /// 
        /// </summary>

        SafeCreateDirectory(Application.dataPath + path);

        StreamWriter sw;
        FileInfo fi;
        fi = new FileInfo(Application.dataPath + path + fileName + ".csv");
        sw = fi.AppendText();

        // position(x,y,z), rotation(quaternion x,y,z,w), 接触判定, ボールの出現判定, ボール番号, ボールに接触した部位
        sw.WriteLine("name,x,y,z,qx,qy,qz,qw,isTouch,isAppear,ballNum,hitObjNum,hitObjName");

        foreach (KeyValuePair<string, List<MotionRecorder.MotionData>> pair in dat)
        {
            foreach (MotionRecorder.MotionData value in pair.Value)
            {
                sw.WriteLine(pair.Key + "," + value.position.x + "," + value.position.y + "," + value.position.z + "," +
                                    value.rotation.x + "," + value.rotation.y + "," + value.rotation.z + "," + value.rotation.w + "," +
                                    value.isTouch + "," + value.isObjAppear + "," + value.ballNum + "," + value.hitObjNum + "," + value.hitObjName);
            }
        }
        sw.Flush();
        sw.Close();
    }
    */

    public static DirectoryInfo SafeCreateDirectory(string path)
    {
        /// <summary>
        /// 指定したパスにディレクトリが存在しない場合
        /// すべてのディレクトリとサブディレクトリを作成する
        /// 
        /// Parameter
        /// ----------
        /// path: string
        ///     ディレクトリを作成するパス
        /// </summary>
        if (Directory.Exists(path))
        {
            return null;
        }
        return Directory.CreateDirectory(path);
    }

}
