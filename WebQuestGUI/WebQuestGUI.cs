using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using System.Runtime.InteropServices;
using ScintillaNET;
namespace WebQuestGUI
{
    public partial class WebQuestGUI : Form
    {
        string EXECUTABLE_PATH=Application.StartupPath +"\\WebQuest.exe";
        string SCRIPT_PATH = Application.StartupPath + "\\script.wq";
        //string[] WEBQUEST_KEYWORDS = {"for","in","while","end","def","return","break"};
        //string[] WEBQUEST_OPERATORS = { "[","]","(",")","+","-","*","/","="};
        //Timer UpdateTimer = new Timer();
        //HighLighter TextBoxHighLighter = new HighLighter();

        public const int StyleDefault = 0;
        public const int StyleKeyword = 1;
        public const int StyleIdentifier = 2;
        public const int StyleNumber = 3;
        public const int StyleString = 4;

        private const int STATE_UNKNOWN = 0;
        private const int STATE_IDENTIFIER = 1;
        private const int STATE_NUMBER = 2;
        private const int STATE_STRING = 3;
        public WebQuestGUI()
        {
            InitializeComponent();
            editor.Margins[0].Type = MarginType.Number;
            editor.Margins[0].Width = 35;

            editor.Lexer = Lexer.Python;

            editor.SetKeywords(0, "for in while if elseif end for def null break");
            editor.SetKeywords(1, "get post post_form post_json sleep append");
            editor.Styles[Style.Default].Size = 12;
            editor.Styles[Style.Default].Font = "CONSOLAS";
            editor.Styles[Style.Python.String].ForeColor = Color.FromArgb(102, 25, 0);
            editor.Styles[Style.Python.Number].ForeColor = Color.FromArgb(158, 0, 126);
            editor.Styles[Style.Python.Operator].ForeColor = Color.FromArgb(0, 102, 27);
            editor.Styles[Style.Python.Operator].Bold = true;
            editor.Styles[Style.Python.DefName].ForeColor = Color.RoyalBlue;
            editor.Styles[Style.Python.CommentLine].ForeColor = Color.Gray;
            editor.Styles[Style.Python.Identifier].ForeColor = Color.Black;
            editor.Styles[Style.Python.Word].ForeColor = Color.FromArgb(0, 49, 186);
            editor.Styles[Style.Python.Word].Bold = true;
            editor.Styles[Style.Python.Word2].ForeColor = Color.Blue;
            editor.Styles[Style.Python.Word2].Italic = true;
            // editor.StyleNeeded += scintilla_StyleNeeded;
            //this.Controls.Add(editor);
        }


        private void btnRun_Click(object sender, EventArgs e)
        {
            var filePath = SCRIPT_PATH;
            
            tbxResult.Clear();
            var writer = File.CreateText(filePath);
            writer.Write(editor.Text);
            writer.Close();

            Task.Run(() => { 
                RunCode();
            });
        }
        private void RunCode()
        {

            //--
            var cmd = EXECUTABLE_PATH + " " + SCRIPT_PATH;
            ProcessStartInfo info = new ProcessStartInfo(EXECUTABLE_PATH);
            info.Arguments = SCRIPT_PATH;
            info.RedirectStandardOutput = true;
            info.UseShellExecute = false;
            info.CreateNoWindow = true;
            info.WindowStyle = ProcessWindowStyle.Hidden;
            using (Process process = Process.Start(info))
            {

                using (StreamReader reader = process.StandardOutput)
                {
                    char[] buffer =new char[1024];
                    int byteread=0;
                    //string result = reader.ReadToEnd();
                    while (!reader.EndOfStream)
                    {
                        byteread = reader.Read(buffer, 0, 1024);
                        //string result = System.Text.Encoding.UTF8.GetString(buffer, 0, byteread);
                        string result = new string(buffer, 0, byteread);
                        Action act = () => { tbxResult.AppendText(result); tbxResult.Refresh(); };
                        this.Invoke(act);
                    }
                    
                }
            }
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            editor.Text= File.ReadAllText(SCRIPT_PATH);
           // (new FrmDocumentation()).Show();
        }



        private void tbxScript_TextChanged(object sender, EventArgs e)
        {
            //HighLight(tbxScript);
            //tbxScript.Refresh();
        }

        private void tbxResult_TextChanged(object sender, EventArgs e)
        {

        }

        private void tbxSearchDoc_TextChanged(object sender, EventArgs e)
        {
            FrmDocumentation.OpenWindow(tbxSearchDoc.Text);
        }

    }
    class HighLighter
    {
        
        private const int WM_SETREDRAW      = 0x000B;
        private const int WM_USER           = 0x400;
        private const int EM_GETEVENTMASK   = (WM_USER + 59);
        private const int EM_SETEVENTMASK   = (WM_USER + 69);
        [DllImport("user32", CharSet = CharSet.Auto)]
        private extern static IntPtr SendMessage(IntPtr hWnd, int msg, int wParam, IntPtr lParam);
        IntPtr eventMask = IntPtr.Zero;
        public void ResetHighLightText( RichTextBox myRtb)
        {
            try
            {
                // Stop redrawing:
                SendMessage(myRtb.Handle, WM_SETREDRAW, 0, IntPtr.Zero);
                // Stop sending of events:
                eventMask = SendMessage(myRtb.Handle, EM_GETEVENTMASK, 0, IntPtr.Zero);
                // change colors and stuff in the RichTextBox

                int positon = myRtb.SelectionStart;
                myRtb.SelectionStart = 0;
                myRtb.SelectionLength = myRtb.Text.Length;
                myRtb.SelectionColor = Color.Black;
                myRtb.SelectionStart = positon;
            }
            finally
            {
                // turn on events
                SendMessage(myRtb.Handle, EM_SETEVENTMASK, 0, eventMask);
                // turn on redrawing
                SendMessage(myRtb.Handle, WM_SETREDRAW, 1, IntPtr.Zero);
            }
        }
        public void HighlightText(RichTextBox myRtb, string word, Color color)
        {

            if (word == "")
            {
                return;
            }
            int s_start = myRtb.SelectionStart, startIndex = 0, index;

            while ((index = myRtb.Text.IndexOf(word, startIndex)) != -1)
            {

                try
                {
                    // Stop redrawing:
                    SendMessage(myRtb.Handle, WM_SETREDRAW, 0, IntPtr.Zero);
                    // Stop sending of events:
                    eventMask = SendMessage(myRtb.Handle, EM_GETEVENTMASK, 0, IntPtr.Zero);
                    // change colors and stuff in the RichTextBox

                
                    myRtb.Select(index, word.Length);
                    myRtb.SelectionColor = color;
                    startIndex = index + word.Length;

                }
                finally
                {
                    // turn on events
                    SendMessage(myRtb.Handle, EM_SETEVENTMASK, 0, eventMask);
                    // turn on redrawing
                    SendMessage(myRtb.Handle, WM_SETREDRAW, 1, IntPtr.Zero);
                }
            }

            myRtb.SelectionStart = s_start;
            myRtb.SelectionLength = 0;
            myRtb.SelectionColor = Color.White;
        }
    }
}
