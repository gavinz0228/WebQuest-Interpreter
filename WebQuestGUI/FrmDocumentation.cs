using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WebQuestGUI.UserControls;
using WebQuestGUI.Models;
namespace WebQuestGUI
{
    public partial class FrmDocumentation : Form
    {
        public static FrmDocumentation SingletonWindow =null;
        public IEnumerable<Documentation> Documentations = Documentation.Load();
        public FrmDocumentation()
        {
            InitializeComponent();
            pnlSearchResult.Resize += SearchResultPanelResizeHandler;
        }

        private void FrmDocumentation_Load(object sender, EventArgs e)
        {
        }
        private void UpdateResult(IEnumerable<Documentation> result)
        {
            pnlSearchResult.Controls.Clear();
            foreach (var doc in result)
            {
                var item = new SearchResultTable(doc);
                item.Width = pnlSearchResult.Width - 30;
                item.Click += ResultOnClickHandler;
                pnlSearchResult.Controls.Add(item);
            }
        }
        private void tbxKeyword_TextChanged(object sender, EventArgs e)
        {
            var keyword = tbxKeyword.Text.Trim();
            if (string.IsNullOrEmpty(keyword))
                return;
            var result = PerformSearch(keyword);
            UpdateResult(result);
        }
        private void DisplayDocumentationDetail(Documentation doc)
        {
            tbxDetail.Text = doc.Description;
            lblFunctionName.Text = doc.FunctionName;
            lblUsage.Text = doc.Usage;
            
        }
        private IEnumerable<Documentation> PerformSearch(string keyword)
        {
            return  Documentations.Where(doc => doc.Description.Contains(keyword)
            || doc.FunctionName.Contains(keyword)
            || doc.Usage.Contains(keyword)).Select(doc => doc).ToList();

        }

        public void SetSearchKeyword(string keyword)
        {
            tbxKeyword.Text = keyword;
            tbxKeyword.SelectionStart = tbxKeyword.Text.Length ;
            tbxKeyword.SelectionLength = 0;
        }
        public static void OpenWindow()
        {
            if (SingletonWindow != null)
            {
                if (SingletonWindow.IsDisposed)
                {
                    SingletonWindow = new FrmDocumentation();
                }
                else
                    SingletonWindow.Show();
            }
            else
            {
                SingletonWindow = new FrmDocumentation();
            }
            SingletonWindow.Show();
        }
        public static void OpenWindow(string keyword)
        {
            OpenWindow();
            SetKeyword(keyword);
        }
        public static void SetKeyword(string keyword)
        {
            SingletonWindow.SetSearchKeyword(keyword);
        }
        void SearchResultPanelResizeHandler(object sender, EventArgs e)
        {
            var width = pnlSearchResult.Width;
            foreach (Control con in pnlSearchResult.Controls)
            {
                con.Width = width - 30;
            }
        }
        void ResultOnClickHandler(object sender, EventArgs e)
        {
            if (sender is SearchResultTable)
            {
                var tbResult = (SearchResultTable)sender;
                DisplayDocumentationDetail(tbResult.BindedData);
                
            }
        }
    }
}
