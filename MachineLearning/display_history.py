import constants as const
import pickle
import matplotlib.pyplot as plt

with open(const.history_path, 'rb') as h: # saving the history of the model
    history = pickle.load(h)
    print (history.keys())

    f, (p1, p2, p3) = plt.subplots(3,1)
    
    # summarize history for accuracy
    p1.plot(history['acc'])
    p1.plot(history['val_acc'])
    p1.set_title('model accuracy')
    p1.set_ylabel('accuracy')
    p1.set_xlabel('epoch')
    p1.legend(['train', 'test'], loc='upper left')
    # summarize history for loss
    p2.plot(history['loss'])
    p2.plot(history['val_loss'])
    p2.set_title('model loss')
    p2.set_ylabel('loss')
    p2.set_xlabel('epoch')
    p2.legend(['train', 'test'], loc='upper left')
    # summarize history for MEA
    p3.plot(history['mean_absolute_error'])
    p3.plot(history['val_mean_absolute_error'])
    p3.set_title('model MEA')
    p3.set_ylabel('MEA')
    p3.set_xlabel('epoch')
    p3.legend(['train', 'test'], loc='upper left')

    plt.tight_layout()
    plt.show()