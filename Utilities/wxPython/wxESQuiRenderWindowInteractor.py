import vtk.wx.wxVTKRenderWindowInteractor as wxRWI

baseRWI = wxRWI.wxVTKRenderWindowInteractor
class wxESQuiRenderWindowInteractor(baseRWI):
  def __init__(self, parent, id):
    baseRWI.__init__(self, parent, id)
    self.scenario = None

  def OnKeyDown(self, event):
    """Handles the wx.EVT_KEY_DOWN event for
    wxVTKRenderWindowInteractor. Specific vtkESQui implementation.
    """

    shift = event.ShiftDown()
    keycode = event.GetKeyCode()
    
    allowed = range(ord('0'),ord('9')) + [ord('c'), ord('q'), ord('z'), ord('x'), \
      wx.WXK_LEFT, wx.WXK_RIGHT, wx.WXK_UP, wx.WXK_DOWN, wx.WXK_PAGEUP, wx.WXK_PAGEDOWN]
    
    # Only process allowed keys
    if not shift and keycode not in allowed:
      return
    
    # event processing should continue
    event.Skip()

    ctrl, shift = event.ControlDown(), event.ShiftDown()
    keycode, keysym = event.GetKeyCode(), 'None'

    # Special keys
    keysym_dict = {wx.WXK_LEFT:'Left', wx.WXK_RIGHT:'Right', \
                   wx.WXK_UP:'Up', wx.WXK_DOWN:'Down', \
                  wx.WXK_PAGEUP:'Prior', wx.WXK_PAGEDOWN:'Next'}
    key = chr(0)
    if keycode < 256:
      key = chr(keycode)
      keysym = key
    elif keycode in keysym_dict:
      keysym = keysym_dict[keycode]

    # wxPython 2.6.0.1 does not return a valid event.Get{X,Y}()
    # for this event, so we use the cached position.
    (x,y)= self._Iren.GetEventPosition()
    self._Iren.SetEventInformation(x, y,
                     ctrl, shift, key, 0,
                     keysym)

    self._Iren.KeyPressEvent()
    self._Iren.CharEvent()
  
  def OnSize(self, event):
    if (self.scenario):
      try:
        width, height = event.GetSize()
      except:
        width = event.GetSize().width
        height = event.GetSize().height
      self.scenario.SetWindowSize(width, height)
      
    baseRWI.OnSize(self, event)

