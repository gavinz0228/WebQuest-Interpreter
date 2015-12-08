namespace WebQuestGUI
{
    partial class WebQuestGUI
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tbxResult = new System.Windows.Forms.TextBox();
            this.btnRun = new System.Windows.Forms.Button();
            this.editor = new ScintillaNET.Scintilla();
            this.scSplitPanel = new System.Windows.Forms.SplitContainer();
            this.lblSearchDoc = new System.Windows.Forms.Label();
            this.tbxSearchDoc = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.scSplitPanel)).BeginInit();
            this.scSplitPanel.Panel1.SuspendLayout();
            this.scSplitPanel.Panel2.SuspendLayout();
            this.scSplitPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // tbxResult
            // 
            this.tbxResult.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbxResult.BackColor = System.Drawing.SystemColors.ControlLight;
            this.tbxResult.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tbxResult.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbxResult.Location = new System.Drawing.Point(3, -2);
            this.tbxResult.Multiline = true;
            this.tbxResult.Name = "tbxResult";
            this.tbxResult.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbxResult.Size = new System.Drawing.Size(996, 136);
            this.tbxResult.TabIndex = 1;
            this.tbxResult.TextChanged += new System.EventHandler(this.tbxResult_TextChanged);
            // 
            // btnRun
            // 
            this.btnRun.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnRun.Location = new System.Drawing.Point(891, 242);
            this.btnRun.Name = "btnRun";
            this.btnRun.Size = new System.Drawing.Size(108, 28);
            this.btnRun.TabIndex = 2;
            this.btnRun.Text = "Run";
            this.btnRun.UseVisualStyleBackColor = true;
            this.btnRun.Click += new System.EventHandler(this.btnRun_Click);
            // 
            // editor
            // 
            this.editor.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.editor.Location = new System.Drawing.Point(-1, -2);
            this.editor.Name = "editor";
            this.editor.Size = new System.Drawing.Size(1007, 237);
            this.editor.TabIndex = 3;
            this.editor.UseTabs = false;
            // 
            // scSplitPanel
            // 
            this.scSplitPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.scSplitPanel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.scSplitPanel.Location = new System.Drawing.Point(0, 0);
            this.scSplitPanel.Name = "scSplitPanel";
            this.scSplitPanel.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // scSplitPanel.Panel1
            // 
            this.scSplitPanel.Panel1.Controls.Add(this.lblSearchDoc);
            this.scSplitPanel.Panel1.Controls.Add(this.tbxSearchDoc);
            this.scSplitPanel.Panel1.Controls.Add(this.editor);
            this.scSplitPanel.Panel1.Controls.Add(this.btnRun);
            // 
            // scSplitPanel.Panel2
            // 
            this.scSplitPanel.Panel2.Controls.Add(this.tbxResult);
            this.scSplitPanel.Size = new System.Drawing.Size(1008, 426);
            this.scSplitPanel.SplitterDistance = 274;
            this.scSplitPanel.TabIndex = 7;
            // 
            // lblSearchDoc
            // 
            this.lblSearchDoc.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblSearchDoc.AutoSize = true;
            this.lblSearchDoc.Location = new System.Drawing.Point(10, 250);
            this.lblSearchDoc.Name = "lblSearchDoc";
            this.lblSearchDoc.Size = new System.Drawing.Size(122, 13);
            this.lblSearchDoc.TabIndex = 5;
            this.lblSearchDoc.Text = "Search Documentation :";
            // 
            // tbxSearchDoc
            // 
            this.tbxSearchDoc.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.tbxSearchDoc.Location = new System.Drawing.Point(137, 247);
            this.tbxSearchDoc.Name = "tbxSearchDoc";
            this.tbxSearchDoc.Size = new System.Drawing.Size(151, 20);
            this.tbxSearchDoc.TabIndex = 4;
            this.tbxSearchDoc.TextChanged += new System.EventHandler(this.tbxSearchDoc_TextChanged);
            // 
            // WebQuestGUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1008, 437);
            this.Controls.Add(this.scSplitPanel);
            this.Name = "WebQuestGUI";
            this.Text = "WebQuestGUI";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.scSplitPanel.Panel1.ResumeLayout(false);
            this.scSplitPanel.Panel1.PerformLayout();
            this.scSplitPanel.Panel2.ResumeLayout(false);
            this.scSplitPanel.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.scSplitPanel)).EndInit();
            this.scSplitPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox tbxResult;
        private System.Windows.Forms.Button btnRun;
        private ScintillaNET.Scintilla editor;
        private System.Windows.Forms.SplitContainer scSplitPanel;
        private System.Windows.Forms.Label lblSearchDoc;
        private System.Windows.Forms.TextBox tbxSearchDoc;
    }
}

