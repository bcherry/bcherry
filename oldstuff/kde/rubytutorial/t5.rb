require 'Qt4'

class MyWidget < Qt::Widget
   def initialize()
      super()
      quit = Qt::PushButton.new('Quit')
      quit.setFont(Qt::Font.new('Times',18,Qt::Font::Bold))

      lcd = Qt::LCDNumber.new(2)

      slider = Qt::Slider.new(Qt::Horizontal)
      slider.setRange(0,99)
      slider.setValue(10)

      connect(quit, SIGNAL('clicked()'), $qApp, SLOT('quit()'))
      connect(slider, SIGNAL('valueChanged(int)'), lcd, SLOT('display(int)'))

      layout = Qt::VBoxLayout.new()
      layout.addWidget(quit)
      layout.addWidget(lcd)
      layout.addWidget(slider)
      setLayout(layout)
   end
end

app = Qt::Application.new(ARGV)

widget = MyWidget.new()

widget.show()
app.exec()
