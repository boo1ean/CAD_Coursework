﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using WPF_GUI.Helpers;

namespace WPF_GUI.ViewModels
{
    public class AboutPopupViewModel : BaseViewModel
    {
        public AboutPopupViewModel()
        {
            StaticLoader.Mediator.Register(MediatorMessages.ShowAboutPopup, (Action<bool>) this.Show);
        }

        public ICommand Close { get { return new DelegateCommand(OnClose); } }

        private void OnClose(object o)
        {
            this.ShowAbout = false;
        }

        private bool _showAbout;
        public bool ShowAbout
        {
            get { return _showAbout; }
            set
            {
                if (value == _showAbout) return;
                _showAbout = value;
                RaisePropertyChanged(() => ShowAbout);
            }
        }

        public void Show(bool state)
        {
            this.ShowAbout = state;
        }
    }
}
