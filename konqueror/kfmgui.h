/* This file is part of the KDE project
   Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>
 
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
 
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
 
   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/     

#ifndef __kfm_gui_h__
#define __kfm_gui_h__

#include <ktopwidget.h>
#include <kstatusbar.h>
#include <ktoolbar.h>
#include <kmenubar.h>
#include <kpanner.h>
#include <kaccel.h>

#include "kfm.h"
#include "kfmview.h"
#include "kfmpopup.h"
#include "kfm_abstract_gui.h"
#include "kfmguiprops.h"

#include <opPart.h>
#include <opMainWindow.h>
#include <opFrame.h>
#include <openparts_ui.h>
#include <opMenu.h>
#include <opToolBar.h>
#include <opStatusBar.h>

#include <qpixmap.h>
#include <qlist.h>
#include <qpopmenu.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qlayout.h>

#include <string>
#include <list>

class KBookmarkMenu;
class KURLCompletion;

class KfmGui : public QWidget,
		public KfmAbstractGui,
		virtual public OPPartIf,
		virtual public KFM::Part_skel
{
  Q_OBJECT
public:
  KfmGui( const char *_url, QWidget *_parent = 0L );
  ~KfmGui();
  
  virtual void init();
  virtual void cleanUp();

  virtual bool event( const char* event, const CORBA::Any& value );
  bool mappingCreateMenubar( OpenPartsUI::MenuBar_ptr menuBar );
  bool mappingCreateToolbar( OpenPartsUI::ToolBarFactory_ptr factory );
  
  /////////////////////////
  // Overloaded functions from @ref KfmAbstractGUI
  /////////////////////////  

  void setStatusBarText( const char *_text );
  void setLocationBarURL( const char *_url );
  void setUpURL( const char *_url );
  
  void addHistory( const char *_url, int _xoffset, int _yoffset );

  void createGUI( const char *_url );
  
  bool hasUpURL() { return !m_currentView.m_strUpURL.isEmpty(); }
  bool hasBackHistory() { return m_currentView.m_lstBack.size() > 0; }
  bool hasForwardHistory() { return m_currentView.m_lstForward.size() > 0; }

public slots:  
  /////////////////////////
  // MenuBar
  /////////////////////////
  virtual void slotLargeIcons();
  virtual void slotSmallIcons();
  virtual void slotTreeView();
  virtual void slotHTMLView();
  virtual void slotSaveGeometry();
  virtual void slotShowCache();
  virtual void slotShowHistory();
  virtual void slotOpenLocation();
  virtual void slotSplitView();
  virtual void slotConfigureKeys();
  virtual void slotAboutApp();

  /////////////////////////
  // Location Bar
  /////////////////////////
  virtual void slotURLEntered();
  
  /////////////////////////
  // ToolBar
  /////////////////////////
  virtual void slotStop();
  virtual void slotNewWindow();
  virtual void slotUp();
  virtual void slotHome();
  virtual void slotBack();
  virtual void slotForward();
  virtual void slotReload();
  
  /////////////////////////
  // Accel
  /////////////////////////
  void slotFocusLeftView();
  void slotFocusRightView();
  
  /////////////////////////
  // Animated Logo
  /////////////////////////
  void slotAnimatedLogoTimeout();
  void slotStartAnimation();
  void slotStopAnimation();
  
  void slotGotFocus( KfmView* _view );

protected:
  virtual void resizeEvent( QResizeEvent *e );
  
  struct History
  {
    QString m_strURL;
    int m_iXOffset;
    int m_iYOffset;
  };
  
  struct View
  {
    KfmView* m_pView;
    QString m_strUpURL;
    list<History> m_lstBack;
    list<History> m_lstForward;
  };

  void initConfig();
  void initGui();
  void initPanner();
  void initMenu();
  void initStatusBar();
  void initToolBar();
  void initView();
  
  void setViewModeMenu( KfmView::ViewMode _viewMode );

  //fills the current view properties with the specified one
  void fillCurrentView( View _view );

  //fills the properties of the specified view with the current ones
  void saveCurrentView( View _view );

//  OpenPartsUI::Menu_var m_vMenuFile;
//  OpenPartsUI::Menu_var m_vMenuFileNew;
  OpenPartsUI::Menu_var m_vMenuEdit;
  OpenPartsUI::Menu_var m_vMenuView;
  OpenPartsUI::Menu_var m_vMenuBookmarks;
  OpenPartsUI::Menu_var m_vMenuOptions;

  OpenPartsUI::ToolBar_var m_vToolBar;
  OpenPartsUI::ToolBar_var m_vLocationBar;
  
  OpenPartsUI::StatusBar_var m_vStatusBar;
/*  
  KMenuBar *m_pMenu;
  KStatusBar *m_pStatusBar;
  KToolBar* m_pToolbar;
  KToolBar* m_pLocationBar;
  KBookmarkMenu* m_pBookmarkMenu;
  QPopupMenu* m_pViewMenu;
  KURLCompletion* m_pCompletion;
*/  
  KPanner* m_pPanner;
  QWidget* m_pPannerChild0;
  QWidget* m_pPannerChild1;
  QGridLayout* m_pPannerChild0GM;
  QGridLayout* m_pPannerChild1GM;
  
  /**
   * The menu "New" in the "File" menu.
   * Since the items of this menu are not connected themselves
   * we need a pointer to this menu to get information about the
   * selected menu item.
   */
//  KNewMenu *m_pMenuNew;

  View m_leftView;
  View m_rightView;
  View m_currentView;
  
  /**
   * Set to true while the constructor is running.
   * @ref #initConfig needs to know about that.
   */
  bool m_bInit;

  unsigned int m_animatedLogoCounter;
  QTimer m_animatedLogoTimer;

  bool m_bBack;
  bool m_bForward;

  KAccel* m_pAccel;

  QString m_strTmpURL;
    
  static QList<OpenPartsUI::Pixmap>* s_lstAnimatedLogo;
  static QList<KfmGui>* s_lstWindows;
};

#endif
